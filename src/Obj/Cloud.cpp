#include "Obj/Cloud.hpp"

void Cloud :: ReturnMap() {
    SetCoordinate({(m_Transform.translation.x > 0) ? -456.0f : 456.0f , m_Transform.translation.y});
}

void Cloud :: Move() {
    if (m_orientation == Orientation::Left) SetCoordinate(m_Transform.translation + MoveLeft);
    else SetCoordinate(m_Transform.translation + MoveRight);

    if (m_Transform.translation.x > 456.0f || m_Transform.translation.x < -456.0f) ReturnMap();
}



