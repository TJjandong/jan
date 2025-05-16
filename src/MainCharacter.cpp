#include "Main_Character.hpp"

#define PressC Util::Input::IsKeyPressed(Util::Keycode::C)
#define PressX Util::Input::IsKeyPressed(Util::Keycode::X)
#define PressLEFT Util::Input::IsKeyPressed(Util::Keycode::LEFT)
#define PressRIGHT Util::Input::IsKeyPressed(Util::Keycode::RIGHT)
#define PressDOWN Util::Input::IsKeyPressed(Util::Keycode::DOWN)
#define PressUP Util::Input::IsKeyPressed(Util::Keycode::UP)

void MainCharacter::movement(const std::vector<std::shared_ptr<Util::GameObject>>& walls) {
    // 定義移動與物理參數
    const float max_speed = 4.3f;
    const float acceleration = 6.0f;
    const float friction = 4.0f;
    const float Jumpforce = 18.3f;         // 跳躍時的初速度
    const float Dashforce = 30.0f;           // 衝刺時的初速度
    float Gravity = 0.9f;            // 重力加速度，每幀施加
    const float max_fall_speed = -max_speed * 2.4f;  // 限制垂直下落速度
    nearLeftWall  = false;
    nearRightWall = false;

    // 時間
    float dt_ms = Util::Time::GetDeltaTimeMs();
    float dt_s  = dt_ms * 0.001f;

    // 讀取按鍵邊緣
    bool cPressedNow   = PressC;
    bool cJustPressed  = (cPressedNow && !m_CPressedLastFrame);
    m_CPressedLastFrame = cPressedNow;

    bool xPressedNow  = PressX;
    bool xJustPressed  = (xPressedNow && !m_XPressedLastFrame);
    m_XPressedLastFrame = xPressedNow;

    // 儲存原始位置
    glm::vec2 oldPos = GetCoordinate();

    // 1. 側邊碰撞偵測（用來牆滑、抓牆）
    CollisionFlags sideFlags;
    DetectSideCollisions(walls, sideFlags);
    Isgrabbing = (sideFlags.left || sideFlags.right) && !IsGround;

    // 角色碰撞箱 & 探测参数
    glm::vec2 charPos   = GetCoordinate() + glm::vec2{0,10};
    glm::vec2 charSize  = {30,27};
    constexpr float WALL_RANGE = 12.0f;

    // 左、右探测箱
    glm::vec2 probeL_pos  = { charPos.x - WALL_RANGE,   charPos.y };
    glm::vec2 probeL_size = { WALL_RANGE,               charSize.y };
    glm::vec2 probeR_pos  = { charPos.x + charSize.x,   charPos.y };
    glm::vec2 probeR_size = { WALL_RANGE,               charSize.y };

    for (auto &w : walls) {
        auto wall = std::dynamic_pointer_cast<InvisibleWall>(w);
        if (!wall) continue;
        glm::vec2 wallPos  = wall->GetCoordinate();
        glm::vec2 wallSize = wall->m_Transform.scale;

        if (RectOverlap(probeL_pos, probeL_size, wallPos, wallSize))
            nearLeftWall = true;
        if (RectOverlap(probeR_pos, probeR_size, wallPos, wallSize))
            nearRightWall = true;
    }

    // 處理衝刺輸入緩衝（X鍵）
    if ((xJustPressed || m_DashBuffered) && !Dashed) {
        isDashing = true;
        dashTimer = dashDuration;
        Dashed = true;
        m_DashBuffered = false; //  一旦使用過 buffer 就要清掉！
        velocity_y = 0.0f;
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

    CollisionFlags flags;

    DetectSideCollisions(walls, flags);

    // 更新衝刺計時器：當處於衝刺狀態時不受 max_speed 限制
    if (isDashing) {
        dashTimer -= dt_ms;
        if (dashTimer <= 0) {
            isDashing = false;
            // 衝刺結束時可以選擇重置部分狀態
        }
    }

    // 處理水平移動（只有在非衝刺狀態下才會被 clamped）
    if (!isDashing) {
        bool appliedInput = false;

        // 1) 只有在鎖定結束後，才允許按鍵改變 velocity_x
        if (m_WallJumpLockTimer <= 0.0f) {
            if (PressRIGHT) {
                velocity_x += acceleration;
                appliedInput = true;
            } else if (PressLEFT) {
                velocity_x -= acceleration;
                appliedInput = true;
            }

            // 限速
            if (velocity_x >  max_speed) velocity_x =  max_speed;
            if (velocity_x < -max_speed) velocity_x = -max_speed;
        }else {
            if (velocity_x > 0) {
                velocity_x = std::max(max_speed , velocity_x - friction * 0.4f);
            } else if (velocity_x < 0) {
                velocity_x = std::min(-max_speed , velocity_x + friction * 0.4f);
            }
            appliedInput = true;
        }

        // 2) 當「沒有使用按鍵」或「仍在鎖定期」時，套摩擦
        //    也就是：只要 appliedInput == false，就一直摩擦
        if (!appliedInput) {
            if (velocity_x > 0) {
                velocity_x = std::max(0.0f, velocity_x - friction);
            } else if (velocity_x < 0) {
                velocity_x = std::min(0.0f, velocity_x + friction);
            }
        }
    } else {
        Gravity = 0.0f;
        if (velocity_x > max_speed) velocity_x -= friction * 0.6f;
        if (velocity_x < -max_speed) velocity_x += friction * 0.6f;
        if (velocity_y > max_speed) velocity_y -= friction * 0.8f;
        if (velocity_y < -max_speed) velocity_y += friction * 0.8f;
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
            if (!IsGround) {
                Isgrabbing = true;
                if (horizontalFlags.left) nearLeftWall = true;
                if (horizontalFlags.right) nearRightWall = true;
                IsJumping = false;
            }
        }else {
            Isgrabbing = false;
        }
    }

    // --- 郊狼時間（Coyote Time）更新 ---
    if (IsGround) {
        m_CoyoteTime = COYOTE_TIME_TOLERANCE;
    } else {
        // 減去經過的秒數 dt_s
        float dt_s = Util::Time::GetDeltaTimeMs() * 0.001f;
        m_CoyoteTime = std::max(m_CoyoteTime - dt_s, 0.0f);
    }

    // 處理跳躍輸入緩衝：按下 C 後開始計時
    if (cJustPressed) {
        m_JumpBuffered = true;
        m_JumpBufferTime = JUMP_BUFFER_DURATION;
    }
    //只計算按住那瞬間的跳躍緩衝
    if (m_JumpBuffered) {
        m_JumpBufferTime -= dt_s;
        if (m_JumpBufferTime <= 0.0f)
            m_JumpBuffered = false;
    }

    if (m_WallJumpLockTimer > 0.0f) {
        m_WallJumpLockTimer = std::max(0.0f, m_WallJumpLockTimer - dt_s);
    }

    if (m_JumpBuffered && !IsJumping &&((IsGround && m_CoyoteTime > 0.0f) || (!IsGround && (nearLeftWall||nearRightWall)))) {
        // 地面跳
        if (IsGround) {
            velocity_y = Jumpforce;
        } else {
            // 壁跳
            velocity_y = Jumpforce;
            velocity_x = (nearLeftWall ? +Jumpforce : -Jumpforce);

            m_WallJumpLockTimer = WALL_JUMP_LOCK_DURATION;
        }
        IsGround      = false;
        IsJumping     = true;
        m_JumpBuffered= false;
        m_CoyoteTime  = 0.0f;
    }


    // 當角色不在地面時施加重力
    if (!IsGround) {
        if (velocity_y > 2.5f)
            velocity_y -= Gravity - 0.45f;
        if (velocity_y <= 2.5f && velocity_y > 0)
            velocity_y -= Gravity - 0.7f;
        else
            velocity_y -= Gravity;

        if (velocity_y < max_fall_speed)
            velocity_y = max_fall_speed;

        if (Isgrabbing) {
            if (velocity_y < 0) {
                if (velocity_y > max_fall_speed*0.4f)
                    velocity_y -= Gravity * 0.033f;
                else
                    velocity_y = max_fall_speed*0.4f;
            }
        }
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
        auto wall = std::dynamic_pointer_cast<Objects>(wallObj);
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

bool MainCharacter::IfCollidesObject(const std::shared_ptr<Objects>& other) const {
    glm::vec2 posA = GetCoordinate() + glm::vec2{0, 10};
    glm::vec2 sizeA = {30, 27};

    glm::vec2 posB = other->GetCoordinate(); // 另一個物件的座標
    glm::vec2 sizeB = other->m_Transform.scale;

    return RectOverlap(posA, sizeA, posB, sizeB);
}

bool MainCharacter::RectOverlap(const glm::vec2 &a, const glm::vec2 &sizeA,
                                  const glm::vec2 &b, const glm::vec2 &sizeB) {
    return (a.x < b.x + sizeB.x && a.x + sizeA.x > b.x &&
            a.y < b.y + sizeB.y && a.y + sizeA.y > b.y);
}

void MainCharacter::KillInstant() {
    // 把角色搬回起點
    ReSpawn();

    // 歸零速度
    velocity_x = 0.0f;
    velocity_y = 0.0f;

    // 重置跳躍、衝刺等狀態
    IsGround       = false;
    IsJumping      = false;
    Dashed         = false;
    isDashing      = false;
    Isgrabbing     = false;
    nearLeftWall   = false;
    nearRightWall  = false;

    // 重置緩衝、郊狼時間
    m_JumpBuffered = false;
    m_CoyoteTime   = 0.0f;
    m_WallJumpLockTimer = 0.0f;

    // 如果你有播放死亡動畫或音效，可以在這裡呼叫
    // PlayDeathAnimation();
}