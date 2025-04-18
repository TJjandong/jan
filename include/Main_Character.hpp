#ifndef MAIN_CHARACTER_HPP
#define MAIN_CHARACTER_HPP
#include "Characters.hpp"

class MainCharacter : public Characters {
public:
    MainCharacter(const std::string& ImagePath) : Characters(ImagePath) {}

    struct CollisionFlags {
        bool left = false;
        bool right = false;
        bool up = false;
        bool down = false;
    };

    void CheckWallProximity(
        const std::vector<std::shared_ptr<Util::GameObject>>& walls,
        const glm::vec2& pos,
        float charWidth,
        float range,
        bool& nearLeft,
        bool& nearRight) const;

    void DetectSideCollisions(const std::vector<std::shared_ptr<Util::GameObject>> &walls, CollisionFlags &flags) const;

    void movement(const std::vector<std::shared_ptr<Util::GameObject>>& walls);

    static bool RectOverlap(const glm::vec2 &a, const glm::vec2 &sizeA, const glm::vec2 &b, const glm::vec2 &sizeB);

    void SetDashed(bool d) { Dashed = d; };

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

};

#endif //MAIN_CHARACTER_HPP
