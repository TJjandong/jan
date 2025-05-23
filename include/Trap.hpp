// Trap.hpp
#ifndef TRAP_HPP
#define TRAP_HPP

#include "Objects.hpp"
#include "Main_Character.hpp"

class Trap : public Objects {
public:
    enum class Orientation { Down, Up, Left, Right };
    Trap(const glm::vec2& coord, Orientation orientation)
    : Objects("")
    {
        SetCoordinate(coord);
        SetScale(1.0f, 1.0f);
        SetZIndex(40);
    }

    void OnCollide(MainCharacter& player);
};

#endif //TRAP_HPP
