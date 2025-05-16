#include "PhaseResourceManager.hpp"

PhaseResourceManager::PhaseResourceManager() {
    //m_TaskText = std::make_shared<TaskText>();
    m_Background = std::make_shared<BackgroundImage>();
}

void PhaseResourceManager::NextPhase(const int phase) {
    m_Background->NextPhase(phase);
    SetBoundary(phase);
}

void PhaseResourceManager::SetBoundary(const int phase) {
    // 指定牆壁資料的 txt 檔路徑（請根據實際路徑調整）
    std::string wallFilePath = "C:/Users/jan20/jan/Resources/WallMatrix/" + std::to_string(phase) + ".txt";
    std::ifstream file(wallFilePath);
    if (!file) {
        LOG_ERROR("無法開啟牆壁資料檔案: {}", wallFilePath);
        return;
    }

    // 清空現有物件
    m_Walls.clear();
    m_Traps.clear();
    m_Goals.clear();

    std::string line;
    int row = 0;
    const float cellSize = 48.0f; // 每個格子的單位尺寸
    glm::vec2 origin(-408.0f, 408.0f); // 第一個格子（左上角）的座標

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
            }else if (cell == 2) {      //Trap
                glm::vec2 pos = origin + glm::vec2(col * cellSize, -row * cellSize);
                auto trap = std::make_shared<Trap>(pos, Trap::Type::Spike );
                trap->SetCoordinate(pos);
                trap->SetScale(cellSize, cellSize/3);
                trap->SetZIndex(40);
                m_Traps.push_back(trap);
            }else if (cell == 3) {      //Trap
                glm::vec2 pos = origin + glm::vec2(col * cellSize, -row * cellSize);
                auto trap = std::make_shared<Trap>(pos, Trap::Type::SpikeBlock );
                trap->SetCoordinate(pos + glm::vec2{-6.0f, 0});
                trap->SetScale(cellSize * 1.25, cellSize * 1.25);
                trap->SetZIndex(40);
                m_Traps.push_back(trap);
            }else if (cell == 9) {
                // 每一個 9 代表一個「下一關目標」
                glm::vec2 goalPos = origin + glm::vec2(col * cellSize, -row * cellSize);
                auto goal = std::make_shared<Goal>(goalPos);
                goal->SetCoordinate(goalPos);
                goal->SetScale(cellSize, cellSize);
                goal->SetZIndex(60);      // 比牆和陷阱都高
                m_Goals.push_back(goal);
            }
            col++;
        }
        row++;
    }
    file.close();
    LOG_DEBUG("牆壁資料讀取完成，共建立 {} 個隱形牆", m_Walls.size());
    LOG_DEBUG("陷阱資料讀取完成");
}
