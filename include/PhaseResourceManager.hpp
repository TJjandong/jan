#ifndef PHASE_MANAGER_HPP
#define PHASE_MANAGER_HPP

#include "BackgroundImage.hpp"
#include "Obj/Trap.hpp"
#include "Obj/GoalFlag.hpp"
#include "Obj/Bounce.hpp"
#include "Obj/InvisibleWall.hpp"
#include "Obj/WoodBox.hpp"
#include "Obj/Balloon.hpp"
#include "Obj/Cloud.hpp"

#include "Util/Logger.hpp"
#include "Util/GameObject.hpp"

#include <vector>
#include <fstream>
#include <sstream>
#include <string>

class PhaseResourceManager {
public:
    PhaseResourceManager();

    [[nodiscard]] std::vector<std::shared_ptr<Util::GameObject>> GetChildren() const {
        // 所有的背景與牆壁
        std::vector<std::shared_ptr<Util::GameObject>> children;
        children.push_back(m_Background);
        for (auto& wall : m_Walls) {
            children.push_back(wall);
        }

        for (auto& trap : m_Traps) {
            children.push_back(trap);
        }

        for (auto& bounce : m_Bounces) {
            children.push_back(bounce);
        }

        for (auto& goal : m_Goals) {
            children.push_back(goal);
        };

        for (auto& boxes : m_WoodBoxes) {
            if (boxes)
                children.push_back(boxes);
        };

        for (auto& balloons : m_Balloons) {
            if (balloons)
                children.push_back(balloons);
        };

        for (auto& clouds : m_Clouds) {
            if (clouds)
                children.push_back(clouds);
        };
        return children;;
    }

    void NextPhase(const int phase);

    void SetBoundary(const int phase, bool flag);  //wall & trap

    // PhaseResourceManager.hpp
    std::vector<std::shared_ptr<Util::GameObject>> GetWall() const {
        std::vector<std::shared_ptr<Util::GameObject>> walls = m_Walls;

        for (auto& box : m_WoodBoxes) {
            walls.push_back(box);
        }
        for (auto& cloud : m_Clouds) {
            walls.push_back(cloud);
        }
        return walls;
    }


private:
    std::shared_ptr<BackgroundImage> m_Background;
    std::vector<std::shared_ptr<Util::GameObject>> m_Walls;     // 儲存所有牆壁物件
    std::vector<std::shared_ptr<Util::GameObject>> m_Traps;
    std::vector<std::shared_ptr<Util::GameObject>> m_Goals;
    std::vector<std::shared_ptr<Util::GameObject>> m_Bounces;
    std::vector<std::shared_ptr<Util::GameObject>> m_WoodBoxes;
    std::vector<std::shared_ptr<Util::GameObject>> m_Balloons;
    std::vector<std::shared_ptr<Util::GameObject>> m_Clouds;
};


#endif //PHASE_MANAGER_HPP
