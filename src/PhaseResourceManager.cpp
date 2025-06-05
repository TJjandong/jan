#include "PhaseResourceManager.hpp"

PhaseResourceManager::PhaseResourceManager() {
    //m_TaskText = std::make_shared<TaskText>();
    m_Background = std::make_shared<BackgroundImage>();
}

void PhaseResourceManager::NextPhase(const int phase) {
    m_Background->NextPhase(phase);
}

void PhaseResourceManager::SetBoundary(const int phase, bool flag) {
    // 指定牆壁資料的 txt 檔路徑（請根據實際路徑調整）
    std::string wallFilePath = RESOURCE_DIR"/WallMatrix/" + std::to_string(phase) + ".txt";
    std::ifstream file(wallFilePath);
    if (!file) {
        LOG_ERROR("無法開啟牆壁資料檔案: {}", wallFilePath);
        return;
    }

    // 清空現有物件
    m_Walls.clear();
    m_Traps.clear();
    m_Goals.clear();
    m_Bounces.clear();
    m_WoodBoxes.clear();
    m_Balloons.clear();
    m_Clouds.clear();

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
                m_Walls.push_back(wall);
            }else if (cell == 2) {
                if (flag == true) {
                    //Trap下
                    glm::vec2 pos = origin + glm::vec2(col * cellSize, -row * cellSize);
                    auto trap = std::make_shared<Trap>(pos, Trap::Direction::Down );
                    trap -> SetScale(1.0f, 0.33f);
                    m_Traps.push_back(trap);
                }else {
                    glm::vec2 position = origin + glm::vec2(col * cellSize, -row * cellSize);
                    auto wall = std::make_shared<InvisibleWall>(position);
                    wall -> SetScale(1.0f, 0.33f);
                    m_Walls.push_back(wall);
                }
            }else if (cell == 3) {
                //Trap上
                glm::vec2 pos = origin + glm::vec2(col * cellSize, -row * cellSize);
                auto trap = std::make_shared<Trap>(pos, Trap::Direction::Up );
                trap -> SetCoordinate(pos + glm::vec2(0.0f, 32.0f));
                trap -> SetScale(1.0f, 0.33f);
                m_Traps.push_back(trap);
            }else if (cell == 4) {
                // 每一個 4 代表一個「彈簧」
                glm::vec2 BouncePos = origin + glm::vec2(col * cellSize, -row * cellSize);
                auto bounce = std::make_shared<Bounce>(BouncePos);
                bounce->SetCoordinate(BouncePos - glm::vec2{0.0f, 10.0f});
                m_Bounces.push_back(bounce);
            }else if (cell == 5) {
                // 每一個 5 代表一個「木箱」
                glm::vec2 BoxPos = origin + glm::vec2(col * cellSize, -row * cellSize);
                auto box = std::make_shared<WoodBox>(BoxPos);
                m_WoodBoxes.push_back(box);
            }else if (cell == 6) {
                // 每一個 6 代表一個「氣球」
                glm::vec2 BalloonPos = origin + glm::vec2(col * cellSize, -row * cellSize);
                auto balloon = std::make_shared<Balloon>(BalloonPos);
                m_Balloons.push_back(balloon);
            }else if (cell == 7) {
                //Cloud左
                glm::vec2 CloudPos = origin + glm::vec2(col * cellSize, -row * cellSize);
                auto cloud = std::make_shared<Cloud>(CloudPos, Cloud::Direction::Left);
                m_Clouds.push_back(cloud);
            }else if (cell == 8) {
                //Cloud右
                glm::vec2 CloudPos = origin + glm::vec2(col * cellSize, -row * cellSize);
                auto cloud = std::make_shared<Cloud>(CloudPos, Cloud::Direction::Right);
                m_Clouds.push_back(cloud);
            }else if (cell == 9) {
                // 每一個 9 代表一個「下一關目標」
                glm::vec2 goalPos = origin + glm::vec2(col * cellSize, -row * cellSize);
                auto goal = std::make_shared<Goal>(goalPos);
                //goal->SetCoordinate(goalPos);
                m_Goals.push_back(goal);
            }else if (cell == 10) {
                if (flag == true) {
                    //Trap左
                    glm::vec2 pos = origin + glm::vec2(col * cellSize, -row * cellSize);
                    auto trap = std::make_shared<Trap>(pos, Trap::Direction::Left );
                    trap -> SetScale(0.33f, 1.0f);
                    m_Traps.push_back(trap);
                }else {
                    glm::vec2 position = origin + glm::vec2(col * cellSize, -row * cellSize);
                    auto wall = std::make_shared<InvisibleWall>(position);
                    wall -> SetScale(0.33f, 1.0f);
                    m_Walls.push_back(wall);
                }
            }else if (cell == 11) {
                if (flag == true) {
                    //Trap右
                    glm::vec2 pos = origin + glm::vec2(col * cellSize, -row * cellSize);
                    auto trap = std::make_shared<Trap>(pos, Trap::Direction::Right );
                    trap -> SetCoordinate(pos + glm::vec2(32.0f, 0.0f));
                    trap -> SetScale(0.33f, 1.0f);
                    m_Traps.push_back(trap);
                }else {
                    glm::vec2 pos = origin + glm::vec2(col * cellSize, -row * cellSize);
                    auto wall = std::make_shared<InvisibleWall>(pos);
                    wall -> SetCoordinate(pos + glm::vec2(32.0f, 0.0f));
                    wall -> SetScale(0.33f, 1.0f);
                    m_Walls.push_back(wall);
                }
            }
            col++;
        }
        row++;

        for (auto& objbounce : m_Bounces) {
            auto bounce = std::dynamic_pointer_cast<Bounce>(objbounce);
            for (auto& objwood : m_WoodBoxes) {
                auto wood = std::dynamic_pointer_cast<WoodBox>(objwood);
                // 比對 x/y 座標，看彈簧是不是在木箱正上方的一格
                auto bp = bounce->GetCoordinate();
                auto wp = wood->GetCoordinate();
                // 例如：如果彈簧 Y = 木箱 Y + 48（就是上方一格），X 完全相同
                if (glm::abs(bp.x - wp.x) < 30.0f && glm::abs(bp.y - (wp.y + 48.0f)) < 30.0f) {
                    bounce->AttachToWood(wood);
                    break;
                }
            }
        }
    }
    file.close();
    LOG_DEBUG("牆壁資料讀取完成，共建立 {} 個隱形牆", m_Walls.size());
    LOG_DEBUG("陷阱資料讀取完成");
}
