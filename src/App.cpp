#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");

    // 設置角色圖片
    m_madline = std::make_shared<Characters>(GA_RESOURCE_DIR "CharacterImg/standR.png");
    m_madline->SetPosition({100.0f, 100.0f});  // 確保角色在可見範圍
    m_madline->SetZIndex(100);  // 設置角色在頂層
    m_Root.AddChild(m_madline);  // 添加角色到根物件


    // 設置背景
    m_PRM = std::make_shared<PhaseResourceManger>();  // 加載背景等
    m_Root.AddChildren(m_PRM->GetChildren());  // 添加背景


    m_CurrentState = State::UPDATE;
}

void App::Update() {
    
    //TODO: do your things here and delete this line <3
    m_Root.Update();
    m_madline->Draw();

    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        glm::vec2 currentPos = m_madline->GetPosition(); // 獲取長頸鹿位置
        currentPos.x += 3.0f;                            // 調整數值控制移動速度
        m_madline->SetPosition(currentPos);              // 更新長頸鹿的位置
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        glm::vec2 currentPos = m_madline->GetPosition(); // 獲取當前位置
        currentPos.x -= 3.0f; // 調整這個數值來控制移動速度
        m_madline->SetPosition(currentPos); // 更新長頸鹿的位置
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::UP)) {
        glm::vec2 currentPos = m_madline->GetPosition(); // 獲取當前位置
        currentPos.y += 3.0f; // 調整這個數值來控制移動速度
        m_madline->SetPosition(currentPos); // 更新長頸鹿的位置
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::DOWN)) {
        glm::vec2 currentPos = m_madline->GetPosition(); // 獲取當前位置
        currentPos.y -= 3.0f; // 調整這個數值來控制移動速度
        m_madline->SetPosition(currentPos); // 更新長頸鹿的位置
    }
    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
