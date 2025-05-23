#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include <iostream>
#include <ostream>

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Renderer.hpp"
#include "Util/Time.hpp"
#include "Objects.hpp"
#include "PhaseResourceManager.hpp"
#include "AnimatedObjects.hpp"
#include "Main_Character.hpp"
#include "AppUtil.hpp"
#include "Balloon.hpp"

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

    State m_CurrentState = State::START;

    Util::Renderer m_Root;
    Phase m_Phase = Phase::Phase00;

    std::shared_ptr<MainCharacter> m_madline;

    //道具物件等交給PhaseResource生成

    std::shared_ptr<AnimatedObjects> m_strawberry;
    std::shared_ptr<AnimatedObjects> m_FlyingStrawberry;
    std::shared_ptr<AnimatedObjects> m_key;
    std::shared_ptr<AnimatedObjects> m_Balloon;

    std::shared_ptr<PhaseResourceManager> m_PRM;

    bool m_EnterDown = false;

    //AppUtil的友元
    friend void AppUtil::removeObjects(App&);
    friend void AppUtil::LoadPhase(App&);
    friend void AppUtil::TransitionToNextPhase(App&);
};

#endif
