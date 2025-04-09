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

    void DetectSideCollisions(const std::vector<std::shared_ptr<Util::GameObject>> &walls, CollisionFlags &flags) const;

    void movement(const std::vector<std::shared_ptr<Util::GameObject>>& walls);

    static bool RectOverlap(const glm::vec2 &a, const glm::vec2 &sizeA, const glm::vec2 &b, const glm::vec2 &sizeB);

private:
    float velocity_x = 0;
    float velocity_y = 0;
    bool IsGround = false;
    bool IsJumping = false;

};

#endif //MAIN_CHARACTER_HPP
