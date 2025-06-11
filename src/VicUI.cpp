#include "VicUI.hpp"
#include <string>
#include <cstdio>

void VictoryUI::CreateNumberSprites(
    Util::Renderer& root,
    const std::string& str,
    float startX,
    float y,
    std::vector<std::shared_ptr<Objects>>& items
) {
    // 每個字符貼圖寬度與間隔 (可視需求微調)
    constexpr float charWidth = 16.0f;
    constexpr float spacing   = 4.0f;
    float x = startX;
    for (char c : str) {
        std::string path;
        if (c >= '0' && c <= '9') {
            path = std::string(RESOURCE_DIR) + "/Image/Victory/" + c + ".png";
        }else if (c == ':') {
            path = std::string(RESOURCE_DIR) + "/Image/Victory/colon.png";
        }else {
            x += charWidth + spacing;
            continue;
        }
        auto obj = std::make_shared<Objects>(path);
        obj->SetCoordinate({ x, y });
        obj->SetScale(1.0f, 1.0f);
        obj->SetZIndex(100);
        root.AddChild(obj);
        items.push_back(obj);
        x += charWidth + spacing;
    }
}

void VictoryUI::Show(float timeSec, int deaths) {
    // 清除 UI
    Hide();

    //背景
    auto bg = std::make_shared<Objects>(
        std::string(RESOURCE_DIR) + "/Image/Victory/VicMenu.png"
    );
    bg->SetCoordinate({ 0.0f, 270.0f });
    bg->SetZIndex(90);
    m_root.AddChild(bg);
    m_items.push_back(bg);

    //通關時間 hh:mm:ss
    int totalSec = static_cast<int>(timeSec + 0.5f);
    int hh = totalSec / 3600;
    int mm = (totalSec % 3600) / 60;
    int ss = totalSec % 60;
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%02d:%02d:%02d", hh, mm, ss);
    CreateNumberSprites(m_root, buf, -60.0f, 300.0f, m_items);

    //Deaths 標籤
    auto dLabel = std::make_shared<Objects>(
        std::string(RESOURCE_DIR) + "/Image/Victory/DEATHS.png"
    );
    dLabel->SetCoordinate({ -30.0f, 240.0f });
    dLabel->SetZIndex(100);
    m_root.AddChild(dLabel);
    m_items.push_back(dLabel);

    //死亡次數
    CreateNumberSprites(m_root, std::to_string(deaths), 100.0f, 240.0f, m_items);

    LOG_DEBUG("圖片讀取完成");
}

void VictoryUI::Hide() {
    for (auto& obj : m_items) {
        m_root.RemoveChild(obj);
    }
    m_items.clear();
}
