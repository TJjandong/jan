#ifndef PHASE_MANGER_HPP
#define PHASE_MANGER_HPP

#include "Util/GameObject.hpp"
#include "Characters.hpp"
#include "BackgroundImage.hpp"
#include <vector>

class PhaseResourceManger {
public:
    PhaseResourceManger();

    [[nodiscard]] std::vector<std::shared_ptr<Util::GameObject>> GetChildren() const {
        // 所有的背景與牆壁
        std::vector<std::shared_ptr<Util::GameObject>> children;
        children.push_back(m_Background);
        for (auto& wall : m_Walls) {
            children.push_back(wall);
        }
        return children;;
    }

    void NextPhase();

    void SetWall(const int phase);

    std::vector<std::shared_ptr<Util::GameObject>> GetWall() const {
        return m_Walls;
    }

private:
    std::shared_ptr<BackgroundImage> m_Background;
    std::vector<std::shared_ptr<Util::GameObject>> m_Walls;     // 儲存所有牆壁物件
    int m_Phase = 1;
};


#endif //PHASE_MANGER_HPP
