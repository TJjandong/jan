// Balloon.cpp
#include "Balloon.hpp"

void Balloon::OnCollide(MainCharacter& player) {
    if (!m_intact) return;        // 如果已經在崩壞冷卻中，就略過
    m_intact = false;             // 標記成「崩壞中」
    m_timer = BalloonCollapseTime; // 啟動 3.0 秒倒數
    SetVisible(false);
    player.ResetDash();
}

void Balloon::Update(float dt) {
    if (!m_intact) {
        m_timer -= dt;
        if (m_timer <= 0.0f) {
            // 倒數結束，還原為原本貼圖
            SetVisible(true);
            m_intact = true;
        }
    }
}