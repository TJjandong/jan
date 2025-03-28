#include "Characters.hpp"
#include "util/Input.hpp"
#include "Main_Character.hpp"
#include <string>
#include <iostream>
#include <cmath>



void MainCharacter::movement(const std::vector<std::shared_ptr<Util::GameObject>>& walls) {
    // 先記錄上一個安全位置
    glm::vec2 oldPos = GetCoordinate();

    const float max_speed = 5.0f;     // 設定最大速度
    const float acceleration = 4.0f;  // 設定加速度
    const float friction = 4.0f;      // 設定摩擦力（減速值）

    // 依照輸入更新速度（這部分保持不變）
    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        if (IfCollidesWall(walls) != CollidedWall::Right) {
            velocity_x += acceleration;
            if (velocity_x > max_speed)
                velocity_x = max_speed;
        }
    } else if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        if (IfCollidesWall(walls) != CollidedWall::Left) {
            velocity_x -= acceleration;
            if (velocity_x < -max_speed)
                velocity_x = -max_speed;
        }
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::UP)) {
        if (IfCollidesWall(walls) != CollidedWall::Up) {
            velocity_y += acceleration;
            if (velocity_y > max_speed)
                velocity_y = max_speed;
        }
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::DOWN)) {
        if (IfCollidesWall(walls) != CollidedWall::Down) {
            velocity_y -= acceleration;
            if (velocity_y < -max_speed)
                velocity_y = -max_speed;
        }
    }

    // 如果沒有按鍵，施加摩擦力
    if (!Util::Input::IsKeyPressed(Util::Keycode::RIGHT) && !Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        if (velocity_x > 0) {
            velocity_x -= friction;
            if (velocity_x < 0) velocity_x = 0;
        } else if (velocity_x < 0) {
            velocity_x += friction;
            if (velocity_x > 0) velocity_x = 0;
        }
    }
    if (!Util::Input::IsKeyPressed(Util::Keycode::UP) && !Util::Input::IsKeyPressed(Util::Keycode::DOWN)) {
        if (velocity_y > 0) {
            velocity_y -= friction;
            if (velocity_y < 0) velocity_y = 0;
        } else if (velocity_y < 0) {
            velocity_y += friction;
            if (velocity_y > 0) velocity_y = 0;
        }
    }

    // 計算新的位置
    glm::vec2 newPos = oldPos + glm::vec2(velocity_x, velocity_y);
    SetCoordinate(newPos);

    // 如果新位置發生碰撞，回復到舊位置並清零速度
    if (IfCollidesWall(walls) != CollidedWall::None) {
        SetCoordinate(oldPos);
        velocity_x = 0;
        velocity_y = 0;
    }
}


int MainCharacter::IfCollidesWall(const std::vector<std::shared_ptr<Util::GameObject>>& walls) const {
    // 主角的碰撞矩形
    glm::vec2 posA = GetCoordinate() + glm::vec2{0, 10};  // 主角左上角的像素座標
    glm::vec2 sizeA = {30,27}; // 主角的寬與高

    // 逐一檢查每一個牆壁
    for (const auto& wallObj : walls) {
        // 嘗試將牆壁物件轉成 Characters（或 InvisibleWall）
        auto wall = std::dynamic_pointer_cast<Characters>(wallObj);
        if (wall) {
            glm::vec2 posB = wall->GetCoordinate();  // 牆壁左上角
            glm::vec2 sizeB = wall->m_Transform.scale; // 牆壁的寬與高

            // 檢查 AABB 是否重疊
            bool overlapX = posA.x < posB.x + sizeB.x && posA.x + sizeA.x > posB.x;
            bool overlapY = posA.y < posB.y + sizeB.y && posA.y + sizeA.y > posB.y;
            if (overlapX && overlapY) {
                // 計算各自中心點
                glm::vec2 centerA = posA + sizeA * 0.5f;
                glm::vec2 centerB = posB + sizeB * 0.5f;
                float deltaX = centerA.x - centerB.x;
                float deltaY = centerA.y - centerB.y;
                // 合併半寬與半高
                float combinedHalfWidth = (sizeA.x + sizeB.x) * 0.5f;
                float combinedHalfHeight = (sizeA.y + sizeB.y) * 0.5f;
                // 計算重疊距離（穿透量）
                float overlapAmountX = combinedHalfWidth - std::fabs(deltaX);
                float overlapAmountY = combinedHalfHeight - std::fabs(deltaY);

                // 判斷哪一軸的穿透量較小，即碰撞面
                if (overlapAmountX < overlapAmountY) {
                    // 水平碰撞
                    if (deltaX > 0) {
                        // 主角中心在牆壁右側，表示碰撞在主角左側
                        return CollidedWall::Left;
                    } else {
                        // 主角中心在牆壁左側，表示碰撞在主角右側
                        return CollidedWall::Right;
                    }
                } else {
                    // 垂直碰撞
                    if (deltaY > 0) {
                        // 主角中心在牆壁下方，表示碰撞在主角上方
                        return CollidedWall::Down;
                    } else {
                        // 主角中心在牆壁上方，表示碰撞在主角下方
                        return CollidedWall::Up;
                    }
                }
            }
        }
    }
    return CollidedWall::None;
}