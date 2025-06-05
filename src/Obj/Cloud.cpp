#include "Obj/Cloud.hpp"

void Cloud :: ReturnMap() {
    SetCoordinate({(GetCoordinate().x > 0) ? -456.0f : 456.0f , GetCoordinate().y});
}

void Cloud :: Move() {
    if (m_direction == Direction::Left) SetCoordinate(GetCoordinate() + MoveLeft);
    else SetCoordinate(GetCoordinate() + MoveRight);

    if (GetCoordinate().x > 456.0f || GetCoordinate().x < -456.0f) ReturnMap();
}



