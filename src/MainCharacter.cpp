#include "Objects.hpp"
#include "util/Input.hpp"
#include "Main_Character.hpp"
#include "Util/Time.hpp"
#include <string>
#include <iostream>
#include <cmath>

#define PressC Util::Input::IsKeyPressed(Util::Keycode::C)
#define PressX Util::Input::IsKeyPressed(Util::Keycode::X)
#define PressLEFT Util::Input::IsKeyPressed(Util::Keycode::LEFT)
#define PressRIGHT Util::Input::IsKeyPressed(Util::Keycode::RIGHT)
#define PressDOWN Util::Input::IsKeyPressed(Util::Keycode::DOWN)
#define PressUP Util::Input::IsKeyPressed(Util::Keycode::UP)

void MainCharacter::movement(const std::vector<std::shared_ptr<Util::GameObject>>& walls) {
    // 儲存原始位置
    glm::vec2 oldPos = GetCoordinate();

    // 處理跳躍輸入緩衝：按下 C 後開始計時
    if (PressC) {
        m_JumpBuffered = true;
        m_JumpBufferTime = JUMP_BUFFER_DURATION;
    } else if (m_JumpBuffered) {
        m_JumpBufferTime -= Util::Time::GetDeltaTime();
        if (m_JumpBufferTime <= 0.0f)
            m_JumpBuffered = false;
    }

    // 處理衝刺輸入緩衝（X鍵）
    if ((PressX || m_DashBuffered) && !Dashed) {
        isDashing = true;
        dashTimer = dashDuration;
        Dashed = true;
        m_DashBuffered = false; //  一旦使用過 buffer 就要清掉！
        const float Dashforce = 30.0f;

        if (PressUP)
            velocity_y = Dashforce;
        if (PressDOWN)
            velocity_y = -Dashforce;
        if (PressRIGHT)
            velocity_x = Dashforce;
        if (PressLEFT)
            velocity_x = -Dashforce;

        IsJumping = true;
    }



    // 定義移動與物理參數
    const float max_speed = 4.0f;
    const float acceleration = 6.0f;
    const float friction = 4.0f;
    const float Jumpforce = 20.0f;         // 跳躍時的初速度
    const float Dashforce = 30.0f;           // 衝刺時的初速度
    float Gravity = 1.0f;            // 重力加速度，每幀施加
    const float max_fall_speed = -max_speed * 2.4f;  // 限制垂直下落速度

    CollisionFlags flags;

    DetectSideCollisions(walls, flags);

    // 更新衝刺計時器：當處於衝刺狀態時不受 max_speed 限制
    if (isDashing) {
        float dt = Util::Time::GetDeltaTimeMs(); // 取得每幀毫秒數
        dashTimer -= dt;
        if (dashTimer <= 0) {
            isDashing = false;
            // 衝刺結束時可以選擇重置部分狀態
        }
    }

    // 處理水平移動（只有在非衝刺狀態下才會被 clamped）
    if (!isDashing) {
        if (PressRIGHT) {
            velocity_x += acceleration;
            if (velocity_x > max_speed)
                velocity_x = max_speed;
        } else if (PressLEFT) {
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
    } else {
        if (velocity_x > max_speed) velocity_x -= friction * 0.6f;
        if (velocity_x < -max_speed) velocity_x += friction * 0.6f;
        if (velocity_y > max_speed) velocity_y -= friction * 0.4f;
        if (velocity_y < -max_speed) velocity_y += friction * 0.4f;
    }

    // 計算新的水平位置
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

    if ((m_JumpBuffered && (m_CoyoteTime > 0.0f || IsGround)) && !flags.up && !IsJumping) {
        velocity_y = Jumpforce;
        IsGround = false;
        IsJumping = true;

        m_CoyoteTime = 0.0f;
        m_JumpBuffered = false;
    }

    // 處理衝刺：按下 X 鍵時啟動衝刺效果
    if ((PressX || m_DashBuffered) && !Dashed) {
        // 啟動衝刺，重設衝刺計時器（單位：毫秒）
        isDashing = true;
        dashTimer = dashDuration;
        Dashed = true;

        // 依照方向給予衝刺速度（這裡會覆蓋原本的水平或垂直速度）
        // 注意：你可以依照需求調整碰撞與優先順序，例如左右方向與上下方向同時選擇時的行為
        if (PressUP)
            velocity_y = Dashforce;
        if (PressDOWN)
            velocity_y = -Dashforce;
        if (PressRIGHT)
            velocity_x = Dashforce;
        if (PressLEFT)
            velocity_x = -Dashforce;

        // 衝刺通常也會讓角色進入跳躍狀態
        IsJumping = true;
    }

    // 當角色不在地面時施加重力
    if (!IsGround) {
        if (velocity_y >= 0)
            velocity_y -= Gravity - 0.5f;
        if (velocity_y <= 0.5 && velocity_y >=-0.5)
            velocity_y -= Gravity - 0.9f;
        else
            velocity_y -= Gravity;

        if (velocity_y < max_fall_speed)
            velocity_y = max_fall_speed;
    }

    // 更新新的垂直位置
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
                // 衝刺結束後重置衝刺標誌，避免無限衝刺
                Dashed = false;
                isDashing = false;
                IsJumping = false;
            }
        } else if (!verticalFlags.down) {
            IsGround = false;
        }
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
