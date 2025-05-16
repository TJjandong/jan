#ifndef PHASE_MANAGER_HPP
#define PHASE_MANAGER_HPP
#include "Objects.hpp"
#include "Trap.hpp"
#include "GoalFlag.hpp"
#include "BackgroundImage.hpp"
#include "InvisibleWall.hpp"

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

        for (auto& g : m_Goals) {
            children.push_back(g);
        };
        return children;;
    }

    void NextPhase(const int phase);

    void SetBoundary(const int phase);  //wall & trap

    std::vector<std::shared_ptr<Util::GameObject>> GetWall() const {
        return m_Walls;
    }

private:
    std::shared_ptr<BackgroundImage> m_Background;
    std::vector<std::shared_ptr<Util::GameObject>> m_Walls;     // 儲存所有牆壁物件
    std::vector<std::shared_ptr<Util::GameObject>> m_Traps;
    std::vector<std::shared_ptr<Util::GameObject>> m_Goals;
};


#endif //PHASE_MANAGER_HPP
