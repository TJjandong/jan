#ifndef PHASE_MANAGER_HPP
#define PHASE_MANAGER_HPP

#include "Util/GameObject.hpp"
#include "Objects.hpp"
#include "Trap.hpp"
#include "BackgroundImage.hpp"
#include <vector>

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
        return children;;
    }

    void NextPhase();

    void SetWall(const int phase);

    void SetTraps(int phase);

    std::vector<std::shared_ptr<Util::GameObject>> GetWall() const {
        return m_Walls;
    }

private:
    std::shared_ptr<BackgroundImage> m_Background;
    std::vector<std::shared_ptr<Util::GameObject>> m_Walls;     // 儲存所有牆壁物件
    std::vector<std::shared_ptr<Trap>> m_Traps;
    int m_Phase = 1;
};


#endif //PHASE_MANAGER_HPP
