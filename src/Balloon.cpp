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

    m_cycleTimer -= dt;
    if (m_cycleTimer <= 0.0f) {
        m_cycleTimer += 3.0f;
    }

    m_moveTimer += dt;
    if (m_moveTimer >= 0.5f) {
        // 每 0.5 秒移動一步
        m_moveTimer -= 0.5f;

        // 判斷目前是在 3 秒週期的哪一段：
        float elapsed = 3.0f - m_cycleTimer;
        float moveAmount = 6.0f;
        // 「一小格」你可自行調整數值 10.0f
        glm::vec2 pos = GetCoordinate();
        if (elapsed < 1.5f) {
            pos.y -= moveAmount;
        } else {
            pos.y += moveAmount;
        }
        SetCoordinate(pos);
    }
}