// Trap.hpp
#ifndef TRAP_HPP
#define TRAP_HPP

#include "Objects.hpp"
#include "Main_Character.hpp"

class Trap : public Objects {
public:
    enum class Direction { Down, Up, Left, Right };
    Trap(const glm::vec2& coord, Direction direction)
    : Objects("")
    {
        SetCoordinate(coord);
        SetScale(1.0f, 1.0f);
        SetZIndex(10);
    }
};

#endif //TRAP_HPP
