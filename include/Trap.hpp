// Trap.hpp
#ifndef TRAP_HPP
#define TRAP_HPP

#include "Objects.hpp"
#include "Main_Character.hpp"

class Trap : public Objects {
public:
    enum class Type { Spike, Fall /*…*/ };

    Trap(const glm::vec2& coord, Type t)
      : Objects(""), m_Type(t)
    {
        SetCoordinate(coord);
        // 例如用 48×48 的碰撞箱
        SetScale(48.0f, 48.0f);
        SetZIndex(30);
    }

    // 在主角碰撞到它時由遊戲主迴圈呼叫
    void OnCollide(MainCharacter& player) {
        switch(m_Type) {
            //case Type::Spike:  player.KillInstant();      break;
            //case Type::Fall:   player.KillInstant();      break;
        }
    }

private:
    Type m_Type;
};

#endif //TRAP_HPP