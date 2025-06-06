// Bounce.cpp
#include "Obj/Bounce.hpp"

void Bounce::OnCollide(MainCharacter& player) {
    player.BounceJump();

    SetImage(m_activeImage);
    SetCoordinate(m_Pos - glm::vec2{0.0f, 20.0f});
    m_timer = BounceTransTime;

    if (auto WoodPtr = m_parentWood.lock())
        WoodPtr->OnCollide();
}

void Bounce::Update(float dt) {
    if (m_timer > 0.0f) {
        m_timer -= dt;
        if (m_timer <= 0.0f) {
            // 倒數結束，還原為原本貼圖
            SetImage(m_origImage);
            SetCoordinate(m_Pos - glm::vec2{0.0f, 10.0f});
        }
    }

    if (auto WoodPtr = m_parentWood.lock()) {
        if (WoodPtr->GetVisibility()) {
            SetVisible(true);
            SetScale(1.0f, 1.0f);
        }else {
            SetVisible(false);
            SetScale(-1.0f, 1.0f);
        }
    }
}