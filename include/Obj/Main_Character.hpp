#ifndef MAIN_CHARACTER_HPP
#define MAIN_CHARACTER_HPP

#include "util/Input.hpp"
#include "Util/Time.hpp"

#include "Objects.hpp"
#include "AnimatedObjects.hpp"
#include "InvisibleWall.hpp"
#include "WoodBox.hpp"
#include "Cloud.hpp"

enum class Direction {
    Left,
    Right
};

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

    template<typename T>
    bool IfCollidesObject(const std::shared_ptr<T>& other) const {
        // 角色碰撞框：腳底往上 4，寬33，高26
        glm::vec2 posA  = GetCoordinate() + glm::vec2{7, 4};
        glm::vec2 sizeA = {33.0f, 26.0f};

        // 目標物件的 AABB
        glm::vec2 posB  = other->GetCoordinate();
        glm::vec2 sizeB = other->m_Transform.scale * 48.0f;

        return RectOverlap(posA, sizeA, posB, sizeB);
    }

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

    void BounceJump() {
        velocity_y = bounceforce;
        ResetDash();
    }

    void ResetDash() {
        Dashed = false;
    }

    void ActivateMode() {
        CheatMode = CheatMode ? false : true;
        std::cout<<"無限衝刺："<<CheatMode<<std::endl;
    }

    void SetMoveImage();

private:
    Direction dir = Direction::Right;
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
    const float COYOTE_TIME_TOLERANCE = 0.3f; // 最大容許時間（秒）

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

    bool CheatMode = false;
};

#endif //MAIN_CHARACTER_HPP
