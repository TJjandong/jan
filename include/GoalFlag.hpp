#ifndef GOALFLAG_HPP
#define GOALFLAG_HPP

#include "Objects.hpp"
#include "Main_Character.hpp"

class Goal : public Objects {
public:
    Goal(const glm::vec2& coord)
      : Objects("")
    {
        SetCoordinate(coord);
        SetScale(48.0f, 48.0f);
        SetZIndex(30);
    }
};

#endif
