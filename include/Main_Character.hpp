#ifndef MAIN_CHARACTER_HPP
#define MAIN_CHARACTER_HPP
#include "Objects.hpp"

class MainCharacter : public Objects {
public:
    MainCharacter(const std::string& ImagePath) : Objects(ImagePath) {}

    struct CollisionFlags {
        bool left = false;
        bool right = false;
        bool up = false;
        bool down = false;
    };

    void DetectSideCollisions(const std::vector<std::shared_ptr<Util::GameObject>> &walls, CollisionFlags &flags) const;

    void movement(const std::vector<std::shared_ptr<Util::GameObject>>& walls);

    static bool RectOverlap(const glm::vec2 &a, const glm::vec2 &sizeA, const glm::vec2 &b, const glm::vec2 &sizeB);

private:
    float velocity_x = 0;
    float velocity_y = 0;
    bool IsGround = false;
    bool IsJumping = false;
    bool Dashed = false;
    bool isDashing = false;
    bool LeanWall = false;
    bool Isgrabbing = false;
    bool nearLeftWall = false;
    bool nearRightWall = false;
    float dashTimer = 0.0f;           // 單位：毫秒
    const float dashDuration = 200.0f; // 衝刺持續 200 毫秒，可依需求調整

    float m_CoyoteTime = 0.0f; // 剩餘郊狼時間
    const float COYOTE_TIME_TOLERANCE = 1.5f; // 最大容許時間（秒）

    bool m_CPressedLastFrame = false;
    float m_JumpBufferTime = 0.0f;
    const float JUMP_BUFFER_DURATION = 5.0f / 60.0f; // 約 0.083 秒
    bool m_JumpBuffered = false;
    bool m_XPressedLastFrame = false;
    float m_DashBufferTime = 0.0f;
    bool m_DashBuffered = false;

    // 鎖定左右鍵的計時器（秒）
    float m_WallJumpLockTimer = 0.0f;
    // 鎖定時長，可調
    static constexpr float WALL_JUMP_LOCK_DURATION = 0.3f;

};

#endif //MAIN_CHARACTER_HPP
