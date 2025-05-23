// Trap.hpp
#ifndef TRAP_HPP
#define TRAP_HPP

#include "Objects.hpp"
#include "Main_Character.hpp"

class Trap : public Objects {
public:
    enum class Type { Spike, SpikeBlock};
    Trap(const glm::vec2& coord, Type t)
      : Objects(""), m_Type(t)
    {
        SetCoordinate(coord);
        SetScale(1.0f, 1.0f);
        SetZIndex(40);
    }

    void OnCollide(MainCharacter& player);

private:
    Type m_Type;
};

#endif //TRAP_HPP
