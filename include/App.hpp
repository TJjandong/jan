#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export

#include "Util/Renderer.hpp"
#include "Characters.hpp"
#include "PhaseResourceManger.hpp"
#include "AnimatedCharacter.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void ValidTask();

    enum class Phase {
        Phase00,
        Phase01,
        Phase02,
        Phase03,
        Phase04,
        Phase05,
        Phase06,
        Phase07,
        Phase08,
        Phase09,
        Phase10,
        Phase11,
        Phase12,
        Phase13,
        Phase14,
        Phase15,
        Phase16,
        Phase17,
        Phase18,
        Phase19,
        Phase20
    };
private:
    State m_CurrentState = State::START;

    Util::Renderer m_Root;

    std::shared_ptr<Characters> m_madline;
    std::shared_ptr<Characters> m_bounce;
    std::shared_ptr<Characters> m_cloud;
    std::shared_ptr<Characters> m_box;
    std::shared_ptr<Characters> m_chest;
    std::shared_ptr<Characters> m_GoalChest;

    std::shared_ptr<AnimatedCharacter> m_strawberry;
    std::shared_ptr<AnimatedCharacter> m_FlyingStrawberry;
    std::shared_ptr<AnimatedCharacter> m_key;
    std::shared_ptr<AnimatedCharacter> m_Balloon;

    std::shared_ptr<PhaseResourceManger> m_PRM;

    bool m_EnterDown = false;
};

#endif
