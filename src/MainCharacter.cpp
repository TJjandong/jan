#include "Characters.hpp"
#include "util/Input.hpp"
#include "Main_Character.hpp"
#include <string>
#include <iostream>
#include <cmath>

void MainCharacter::movement(const std::vector<std::shared_ptr<Util::GameObject>>& walls) {
    // 先記錄原始安全位置
    glm::vec2 oldPos = GetCoordinate();

    const float max_speed = 5.0f;
    const float acceleration = 4.0f;
    const float friction = 4.0f;

    CollisionFlags flags;
    DetectSideCollisions(walls, flags);  // 初次偵測

    // 更新水平方向速度
    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        if (!flags.right) {  // 如果右側沒有碰撞
            velocity_x += acceleration;
            if (velocity_x > max_speed)
                velocity_x = max_speed;
        }
    } else if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        if (!flags.left) {   // 如果左側沒有碰撞
            velocity_x -= acceleration;
            if (velocity_x < -max_speed)
                velocity_x = -max_speed;
        }
    }
    // 更新垂直方向速度
    if (Util::Input::IsKeyPressed(Util::Keycode::UP)) {
        if (!flags.up) {     // 如果上方沒有碰撞
            velocity_y += acceleration;
            if (velocity_y > max_speed)
                velocity_y = max_speed;
        }
    } else if (Util::Input::IsKeyPressed(Util::Keycode::DOWN)) {
        if (!flags.down) {   // 如果下方沒有碰撞
            velocity_y -= acceleration;
            if (velocity_y < -max_speed)
                velocity_y = -max_speed;
        }
    }

    // 當沒有水平按鍵時施加摩擦
    if (!Util::Input::IsKeyPressed(Util::Keycode::RIGHT) && !Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        if (velocity_x > 0) {
            velocity_x -= friction;
            if (velocity_x < 0) velocity_x = 0;
        } else if (velocity_x < 0) {
            velocity_x += friction;
            if (velocity_x > 0) velocity_x = 0;
        }
    }
    // 當沒有垂直按鍵時施加摩擦
    if (!Util::Input::IsKeyPressed(Util::Keycode::UP) && !Util::Input::IsKeyPressed(Util::Keycode::DOWN)) {
        if (velocity_y > 0) {
            velocity_y -= friction;
            if (velocity_y < 0) velocity_y = 0;
        } else if (velocity_y < 0) {
            velocity_y += friction;
            if (velocity_y > 0) velocity_y = 0;
        }
    }

    // 分別檢查水平與垂直碰撞
    glm::vec2 newPos = oldPos;   // 從原始位置開始計算

    // 先更新水平方向
    newPos.x += velocity_x;
    // 對水平移動進行碰撞檢測（僅關注左右）
    {
        // 先儲存原始水平位置
        glm::vec2 testPos = newPos; // 水平移動後的暫時位置
        SetCoordinate({testPos.x, oldPos.y}); // 只更新水平部分
        CollisionFlags horizontalFlags;
        DetectSideCollisions(walls, horizontalFlags);
        if (horizontalFlags.left || horizontalFlags.right) {
            // 若有水平方向碰撞，回復水平位置
            newPos.x = oldPos.x;
            velocity_x = 0;
        }
    }

    // 接著更新垂直方向
    newPos.y += velocity_y;
    {
        glm::vec2 testPos = newPos;
        SetCoordinate({newPos.x, testPos.y}); // 只更新垂直部分
        CollisionFlags verticalFlags;
        DetectSideCollisions(walls, verticalFlags);
        if (verticalFlags.up || verticalFlags.down) {
            newPos.y = oldPos.y;
            velocity_y = 0;
        }
    }

    // 最終設定角色座標
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

    glm::vec2 upRectPos     = { posA.x, posA.y - eps };
    glm::vec2 upRectSize    = { sizeA.x, eps };

    glm::vec2 downRectPos   = { posA.x, posA.y + sizeA.y };
    glm::vec2 downRectSize  = { sizeA.x, eps };

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