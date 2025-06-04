// WoodBox.cpp
#include "Obj/WoodBox.hpp"

void WoodBox::OnCollide() {
    if (!m_intact) return;        // 如果已經在崩壞冷卻中，就略過
    m_intact = false;             // 標記成「崩壞中」
    m_timer = WoodCollapseTime; // 啟動 3.0 秒倒數
}

void WoodBox::Update(float dt) {
    if (!m_intact) {
        m_timer -= dt;
        if (m_timer <= 0.0f) {
            // 倒數結束，還原為原本貼圖
            SetVisible(true);
            SetImage(m_Image1);
            SetScale(1.0f, 1.0f);
            m_intact = true;
        }else if (m_timer <= 2.0f) {
            // 立刻隱藏木箱：移除碰撞效果＋改成透明
            SetScale(-1.0f, -1.0f);
            SetVisible(false);       // 清掉圖片
        }else if (m_timer <= 2.33f) {
            SetImage(m_Image3);
        }else if (m_timer <= 2.67f) {
            SetImage(m_Image2);
        }
    }
}