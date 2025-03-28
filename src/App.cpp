#include "App.hpp"

#include <iostream>
#include <ostream>

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");


    // 設置角色圖片
    m_madline = std::make_shared<MainCharacter>(GA_RESOURCE_DIR "CharacterImg/standR.png");
    m_madline->SetCoordinate({-100.0f, -100.0f});  // 確保角色在可見範圍
    m_madline->SetZIndex(100);  // 設置角色在頂層
    m_Root.AddChild(m_madline);  // 添加角色到根物件


    // 設置背景
    m_PRM = std::make_shared<PhaseResourceManger>();  // 加載背景等
    m_PRM->SetWall(0);
    m_Root.AddChildren(m_PRM->GetChildren());  // 添加背景

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    
    //TODO: do your things here and delete this line <3
    m_Root.Update();
    m_madline->Draw();

    // Get the current position of the main character
    glm::vec2 currentPos = m_madline->GetPosition();


    std::string coordinatesText = "X: " + std::to_string(currentPos.x) + " Y: " + std::to_string(currentPos.y);

    // Use the renderer to draw the text on the screen
    // Assuming m_Root.AddText() is a function that adds text to be rendered on the screen
    std::cout << coordinatesText << std::endl;

    m_madline->movement(m_PRM->GetWall());

    // 假設你在 PhaseResourceManger 中另外提供 GetWalls() 方法，只回傳牆壁 vector
    if (m_madline->IfCollidesWall(m_PRM->GetWall())) {
        // 處理碰撞，例如停止移動或回復到上一步
        std::cout << "主角與隱形牆發生碰撞！" << std::endl;
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
