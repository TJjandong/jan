#ifndef MAIN_CHARACTER_HPP
#define MAIN_CHARACTER_HPP
#include "Objects.hpp"
#include "AnimatedObjects.hpp"
#include "util/Input.hpp"
#include "Util/Time.hpp"
#include "InvisibleWall.hpp"
#include "WoodBox.hpp"
#include "Cloud.hpp"
#include <string>
#include <iostream>
#include <cmath>

class MainCharacter : public Objects {
public:
    MainCharacter(const std::string& ImagePath) : Objects(ImagePath) {}

    struct CollisionFlags {
        bool left = false;
        bool right = false;
        bool up = false;
        bool down = false;
    };

    // 水平碰撞並移動，回傳是否有任何左右碰撞
    CollisionFlags MoveX(const std::vector<std::shared_ptr<Util::GameObject>>& walls, float dx);

    // 垂直碰撞並移動，回傳是否有上下碰撞
    CollisionFlags MoveY(const std::vector<std::shared_ptr<Util::GameObject>>& walls, float dy);

    std::shared_ptr<Util::GameObject> FindCollision(
        const std::vector<std::shared_ptr<Util::GameObject>>& walls,
        const glm::vec2& testPos
    );

    bool IfCollidesObject(const std::shared_ptr<Objects>& other) const;

    bool IfCollidesObject(const std::shared_ptr<AnimatedObjects>& other) const;

    void movement(const std::vector<std::shared_ptr<Util::GameObject>>& walls);

    static bool RectOverlap(const glm::vec2 &a, const glm::vec2 &sizeA, const glm::vec2 &b, const glm::vec2 &sizeB);

    void KillInstant();

    void SetSpawnPoint(const glm::vec2 &spawn) {
        SetCoordinate(spawn);
        ResetCoordinate = spawn;
    }

    void ReSpawn() {
        m_Transform.translation = ResetCoordinate;
    }

    void BounceJump();

    void ResetDash();

private:
    glm::vec2 ResetCoordinate;
    float velocity_x = 0;
    float velocity_y = 0;
    bool IsGround = false;
    bool IsJumping = false;
    bool Dashed = false;
    bool isDashing = false;
    bool Isgrabbing = false;
    bool nearLeftWall = false;
    bool nearRightWall = false;
    float dashTimer = 0.0f;           // 單位：毫秒
    const float dashDuration = 250.0f; // 衝刺持續 200 毫秒，可依需求調整
    float bounceforce = 23.3f;

    float m_CoyoteTime = 0.0f; // 剩餘郊狼時間
    const float COYOTE_TIME_TOLERANCE = 1.5f; // 最大容許時間（秒）

    bool m_CPressedLastFrame = false;
    float m_JumpBufferTime = 0.0f;
    const float JUMP_BUFFER_DURATION = 5.0f / 60.0f; // 約 0.083 秒
    bool m_JumpBuffered = false;
    bool m_XPressedLastFrame = false;
    bool m_DashBuffered = false;

    // 鎖定左右鍵的計時器（秒）
    float m_WallJumpLockTimer = 0.0f;
    // 鎖定時長，可調
    static constexpr float WALL_JUMP_LOCK_DURATION = 0.25f;

};

#endif //MAIN_CHARACTER_HPP
