#include "PhaseResourceManager.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Util/Logger.hpp"
#include "Util/Image.hpp"
#include "InvisibleWall.hpp"
#include "Objects.hpp"

PhaseResourceManager::PhaseResourceManager() {
    //m_TaskText = std::make_shared<TaskText>();
    m_Background = std::make_shared<BackgroundImage>();
}

void PhaseResourceManager::NextPhase() {
    if (m_Phase == 21) return;
    LOG_DEBUG("Passed! Next phase: {}", m_Phase);
    m_Background->NextPhase(m_Phase);
    //m_TaskText->NextPhase(m_Phase++);
}

void PhaseResourceManager::SetWall(const int phase) {
    // 指定牆壁資料的 txt 檔路徑（請根據實際路徑調整）
    std::string wallFilePath = "C:/Users/USER/Desktop/Celeste/ptsd-template/Resources/WallMatrix/" + std::to_string(phase) + ".txt";
    std::ifstream file(wallFilePath);
    if (!file) {
        LOG_ERROR("無法開啟牆壁資料檔案: {}", wallFilePath);
        return;
    }

    // 清空現有牆壁物件
    m_Walls.clear();

    std::string line;
    int row = 0;
    const float cellSize = 48.0f; // 每個格子的單位尺寸
    glm::vec2 origin(-408.0f, 360.0f); // 第一個格子（左上角）的座標

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int cell;
        int col = 0;
        while (iss >> cell) {
            if (cell == 1) {
                glm::vec2 position = origin + glm::vec2(col * cellSize, -row * cellSize);
                auto wall = std::make_shared<InvisibleWall>(position);
                wall->SetCoordinate(position);
                wall->SetScale(cellSize, cellSize);  // 設定牆壁尺寸為48×48單位
                wall->SetZIndex(50);  // 可依需求調整
                m_Walls.push_back(wall);
            }
            col++;
        }
        row++;
    }
    file.close();
    LOG_DEBUG("牆壁資料讀取完成，共建立 {} 個隱形牆", m_Walls.size());
}
