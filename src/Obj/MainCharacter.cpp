#include "Obj/AnimatedObjects.hpp"
#include "Obj/Main_Character.hpp"

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
    const float Jumpforce = 17.5f;         // 跳躍時的初速度
    const float Dashforce = 30.0f;           // 衝刺時的初速度
    float Gravity = 0.6f;            // 重力加速度，每幀施加
    const float max_fall_speed = -max_speed * 2.1f;  // 限制垂直下落速度
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

        if (CheatMode)                              //作弊模式
            ResetDash();

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

        // 當角色不在地面時施加重力
        if (!IsGround) {
            if (velocity_y > 2.5f)
                velocity_y -= Gravity;
            if (velocity_y <= 2.5f && velocity_y >= -2.5f)
                velocity_y -= Gravity - 0.25f;
            else
                velocity_y -= Gravity;

            if (velocity_y < max_fall_speed)
                velocity_y = max_fall_speed;

            if (Isgrabbing) {
                if (velocity_y < 0) {
                    if (velocity_y > max_fall_speed*0.4f)
                        velocity_y -= Gravity * 0.088f;
                    else
                        velocity_y = max_fall_speed*0.4f;
                }
            }
        }
    } else {
        Gravity = 0.0f;
        if (velocity_x > max_speed) velocity_x -= friction * 0.6f;
        if (velocity_x < -max_speed) velocity_x += friction * 0.6f;
        if (velocity_y > max_speed) velocity_y -= friction * 0.7f;
        if (velocity_y < max_fall_speed) velocity_y += friction * 0.7f;
    }


    // 碰撞與移動拆成兩步：水平、再垂直
    auto hFlags = MoveX(walls, velocity_x);
    // 更新抓牆狀態
    nearLeftWall = hFlags.left;
    nearRightWall = hFlags.right;
    Isgrabbing     = (nearLeftWall || nearRightWall) && !IsGround;
    if (Isgrabbing) IsJumping = false;

    // 更新新的垂直位置
    auto vFlags = MoveY(walls, velocity_y);


    // --- 郊狼時間（Coyote Time）更新 ---
    if (IsGround) {
        m_CoyoteTime = COYOTE_TIME_TOLERANCE;
    } else {
        // 減去經過的秒數 dt_s
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

    if (m_JumpBuffered && !IsJumping && (m_CoyoteTime > 0.0f || Isgrabbing)) {
        // 地面跳
        if (IsGround || m_CoyoteTime > 0.0f) {
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

}

MainCharacter:: CollisionFlags MainCharacter::MoveX(const std::vector<std::shared_ptr<Util::GameObject>>& walls, float dx) {
    CollisionFlags flags{};  // all false

    glm::vec2 pos = GetCoordinate();
    glm::vec2 testPos = pos + glm::vec2{dx, 0};

    // 看看未來的位置，跟哪個物件撞
    auto hit = FindCollision(walls, testPos);
    if (hit) {
        if (auto cloud = std::dynamic_pointer_cast<Cloud>(hit)) {
            pos.x += dx;
            SetCoordinate(pos);
            return flags;
        }
        // 左右標旗
        if (dx > 0) flags.right = true;
        else         flags.left  = true;

        // 停下來
        velocity_x = 0.0f;

        // 如果是木箱就摧毀
        if (auto box = std::dynamic_pointer_cast<WoodBox>(hit);
            box && box->IsIntact()){
            box->OnCollide();
        }
    } else {
        // 沒撞：才更新位置
        pos.x += dx;
        SetCoordinate(pos);
    }
    return flags;
}

MainCharacter:: CollisionFlags MainCharacter::MoveY(const std::vector<std::shared_ptr<Util::GameObject>>& walls, float dy) {
    CollisionFlags flags{};

    glm::vec2 pos = GetCoordinate();
    glm::vec2 testPos = pos + glm::vec2{0.0f, dy};

    auto hit = FindCollision(walls, testPos);
    if (hit) {
        if (dy > 0 ) {
            if (auto cloud = std::dynamic_pointer_cast<Cloud>(hit)) {
                // 空中時取消地面狀態
                if (!flags.down) IsGround = false;
                pos.y += dy;
                SetCoordinate(pos);
                return flags;
            }
            flags.up   = true;
        }
        else        flags.down = true;

        velocity_y = 0.0f;


        // 著地時特別處理狀態
        if (flags.down) {
            IsGround   = true;
            Dashed     = false;
            isDashing  = false;
            IsJumping  = false;
        }

        if (auto cloud = std::dynamic_pointer_cast<Cloud>(hit)) {
            if (cloud->IsRight())
                SetCoordinate(pos + MoveRight);
            else
                SetCoordinate(pos + MoveLeft);
        }

        // 木箱摧毀
        if (auto box = std::dynamic_pointer_cast<WoodBox>(hit);
            box && box->IsIntact()){
            box->OnCollide();
        }
    } else {
        // 空中時取消地面狀態
        if (!flags.down) IsGround = false;
        pos.y += dy;
        SetCoordinate(pos);
    }
    return flags;
}

std::shared_ptr<Util::GameObject> MainCharacter::FindCollision(
    const std::vector<std::shared_ptr<Util::GameObject>>& walls,
    const glm::vec2& testPos
){
    // 暫存座標
    auto orig = GetCoordinate();
    SetCoordinate(testPos);

    // 找第一個重疊的
    for (auto& obj : walls) {
        // 只處理繼承自 Objects（或你定義的 InvisibleWall）的物件
        auto wall = std::dynamic_pointer_cast<Objects>(obj);
        if (!wall)
            continue;    // 不是「牆」就跳過

        if (IfCollidesObject(wall)) {
            SetCoordinate(orig);
            return obj;
        }
    }
    // 還原
    SetCoordinate(orig);
    return nullptr;
}

bool MainCharacter::IfCollidesObject(const std::shared_ptr<Objects>& other) const {
    glm::vec2 posA = GetCoordinate() + glm::vec2{0, 10};
    glm::vec2 sizeA = {30, 27};

    glm::vec2 posB = other->GetCoordinate(); // 另一個物件的座標
    glm::vec2 sizeB = other->m_Transform.scale * 48.0f;

    return RectOverlap(posA, sizeA, posB, sizeB);
}

bool MainCharacter::IfCollidesObject(const std::shared_ptr<AnimatedObjects>& other) const {
    glm::vec2 posA = GetCoordinate() + glm::vec2{0, 10};
    glm::vec2 sizeA = {30.0f, 27.0f};

    glm::vec2 posB = other->GetCoordinate(); // 另一個物件的座標
    glm::vec2 sizeB = other->m_Transform.scale * 48.0f;

    return RectOverlap(posA, sizeA, posB, sizeB);
}

bool MainCharacter::RectOverlap(const glm::vec2 &a, const glm::vec2 &sizeA,
                                  const glm::vec2 &b, const glm::vec2 &sizeB) {
    return (a.x < b.x + sizeB.x && a.x + sizeA.x > b.x &&
            a.y < b.y + sizeB.y && a.y + sizeA.y > b.y);
}

