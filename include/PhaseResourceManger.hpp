#ifndef PHASE_MANGER_HPP
#define PHASE_MANGER_HPP

#include "Util/GameObject.hpp"
#include "Characters.hpp"
#include "BackgroundImage.hpp"

class PhaseResourceManger {
public:
    PhaseResourceManger();

    [[nodiscard]] std::vector<std::shared_ptr<Util::GameObject>> GetChildren() const {
        return {m_Background};
    }

    void NextPhase();

private:
    std::shared_ptr<BackgroundImage> m_Background;

    int m_Phase = 1;
};


#endif //PHASE_MANGER_HPP
