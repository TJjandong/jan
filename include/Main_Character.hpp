#ifndef MAIN_CHARACTER_HPP
#define MAIN_CHARACTER_HPP
#include "Characters.hpp"

class MainCharacter : public Characters {
public:
    MainCharacter(const std::string& ImagePath) : Characters(ImagePath) {}

    int IfCollidesWall(const std::vector<std::shared_ptr<Util::GameObject>>& walls) const;

    void movement(const std::vector<std::shared_ptr<Util::GameObject>>& walls);

private:
    enum CollidedWall {
        None,
        Right,
        Left,
        Up,
        Down
    };
    float velocity_x = 0;
    float velocity_y = 0;

};

#endif //MAIN_CHARACTER_HPP
