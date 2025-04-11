#include "Characters.hpp"
#include "util/Input.hpp"
#include "Main_Character.hpp"
#include <string>
#include <iostream>
#include <cmath>

void MainCharacter::movement(const std::vector<std::shared_ptr<Util::GameObject>>& walls) {
    // 儲存原始位置
    glm::vec2 oldPos = GetCoordinate();

    // 定義移動與物理參數
    const float max_speed = 5.0f;
    const float acceleration = 4.0f;
    const float friction = 4.0f;
    const float Jumpforce = 10.0f;         // 跳躍時的初速度大小
    const float Gravity = 0.5f;            // 重力加速度，每幀施加
    const float max_fall_speed = -max_speed * 1.5f;  // 限制垂直下落速度

    CollisionFlags flags;
    DetectSideCollisions(walls, flags);

    // 處理水平移動
    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        velocity_x += acceleration;
        if (velocity_x > max_speed)
            velocity_x = max_speed;
    } else if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        velocity_x -= acceleration;
        if (velocity_x < -max_speed)
            velocity_x = -max_speed;
    } else {
        // 沒有左右按鍵時施加摩擦（水平減速）
        if (velocity_x > 0) {
            velocity_x -= friction;
            if (velocity_x < 0) velocity_x = 0;
        } else if (velocity_x < 0) {
            velocity_x += friction;
            if (velocity_x > 0) velocity_x = 0;
        }
    }

    // 計算新的位置
    glm::vec2 newPos = oldPos;
    newPos.x += velocity_x;
    // 檢查下一步的水平碰撞
    {
        glm::vec2 testPos = newPos;
        SetCoordinate({testPos.x, oldPos.y});
        CollisionFlags horizontalFlags;
        DetectSideCollisions(walls, horizontalFlags);
        if (horizontalFlags.left || horizontalFlags.right) {
            newPos.x = oldPos.x;
            velocity_x = 0;
        }
    }

    // 處理跳躍
    // 當角色在地面上且按下跳躍鍵（這裡使用 C 鍵），且上方沒有阻擋時，就設定初始垂直速度為負值（向上）
    if (Util::Input::IsKeyPressed(Util::Keycode::C)) {
        //std::cout << "Pressed C" << std::endl;
        if (IsGround && !flags.up) {
            velocity_y = Jumpforce;
            IsGround = false;
            IsJumping = true;
        }
    }

    // 當角色不在地面時施加重力（使角色向下加速）
    if (!IsGround || IsJumping) {
        velocity_y -= Gravity;
        if (velocity_y < max_fall_speed)
            velocity_y = max_fall_speed;
    }

    newPos.y += velocity_y;
    // 檢查下一步的垂直碰撞
    {
        glm::vec2 testPos = newPos;
        SetCoordinate({newPos.x, testPos.y});
        CollisionFlags verticalFlags;
        DetectSideCollisions(walls, verticalFlags);
        if (verticalFlags.up || verticalFlags.down) {
            newPos.y = oldPos.y;
            velocity_y = 0;
            if (verticalFlags.down) {
                // 當角色碰到底部（地面），視為在地面上
                IsGround = true;
                std::cout << "IsGround" << std::endl;
            }
            if (verticalFlags.up) std::cout << "IsUp" << std::endl;
        }else if (!verticalFlags.down)
            IsGround = false;
    }

    // 更新角色最終位置
    SetCoordinate(newPos);
}


void MainCharacter::DetectSideCollisions(const std::vector<std::shared_ptr<Util::GameObject>> &walls, CollisionFlags &flags) const {
    // 清空標旗
    flags.left = flags.right = flags.up = flags.down = false;

    // 取得主角碰撞矩形，這裡假設主角尺寸是 {30,27}
    glm::vec2 posA = GetCoordinate() + glm::vec2{0, 10};
    glm::vec2 sizeA = {30, 27};
    const float eps = 1.0f; // 偵測厚度

    // 定義各邊的區域
    glm::vec2 leftRectPos   = { posA.x - eps, posA.y };
    glm::vec2 leftRectSize  = { eps, sizeA.y };

    glm::vec2 rightRectPos  = { posA.x + sizeA.x, posA.y };
    glm::vec2 rightRectSize = { eps, sizeA.y };

    glm::vec2 downRectPos     = { posA.x, posA.y - eps };
    glm::vec2 downRectSize    = { sizeA.x, eps };

    glm::vec2 upRectPos   = { posA.x, posA.y + sizeA.y };
    glm::vec2 upRectSize  = { sizeA.x, eps };

    // 檢查每個牆壁
    for (const auto &wallObj : walls) {
        auto wall = std::dynamic_pointer_cast<Characters>(wallObj);
        if (wall) {
            glm::vec2 posB = wall->GetCoordinate();
            glm::vec2 sizeB = wall->m_Transform.scale;  // 牆壁尺寸
            if (RectOverlap(leftRectPos, leftRectSize, posB, sizeB))
                flags.left = true;
            if (RectOverlap(rightRectPos, rightRectSize, posB, sizeB))
                flags.right = true;
            if (RectOverlap(upRectPos, upRectSize, posB, sizeB))
                flags.up = true;
            if (RectOverlap(downRectPos, downRectSize, posB, sizeB))
                flags.down = true;
        }
    }
}

bool MainCharacter::RectOverlap(const glm::vec2 &a, const glm::vec2 &sizeA,
                                  const glm::vec2 &b, const glm::vec2 &sizeB) {
    return (a.x < b.x + sizeB.x && a.x + sizeA.x > b.x &&
            a.y < b.y + sizeB.y && a.y + sizeA.y > b.y);
}
