#ifndef MAIN_CHARACTER_HPP
#define MAIN_CHARACTER_HPP
#include "Characters.hpp"

class MainCharacter : public Characters {
public:
    MainCharacter(const std::string& ImagePath) : Characters(ImagePath) {}
    void movement();

private:
    float velocity_x = 0;
    float velocity_y = 0;
};

#endif //MAIN_CHARACTER_HPP
