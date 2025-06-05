#include "App.hpp"

#define PressX Util::Input::IsKeyPressed(Util::Keycode::X)
#define PressU Util::Input::IsKeyPressed(Util::Keycode::U)
#define PressI Util::Input::IsKeyPressed(Util::Keycode::I)

void App::Start() {
    LOG_TRACE("Start");


    // 設置角色圖片
    m_madline = std::make_shared<MainCharacter>(RESOURCE_DIR "/Image/Character/standR.png");
    m_madline->SetZIndex(100);

    // 設置背景
    m_Phase = Phase::Phase00;
    m_PRM = std::make_shared<PhaseResourceManager>();  // 加載背景等
    m_PRM->NextPhase(m_Phase);
    AppUtil::LoadPhase(*this);

    m_CurrentState = State::UPDATE;
}

void App::Update() {

    //TODO: do your things here and delete this line <3
    float dt_s = Util::Time::GetDeltaTimeMs() * 0.001f;  // 毫秒轉秒

    if (UbufferTime > 0.0f)
        UbufferTime -= dt_s;
    else
        Ubuffer = false;
    if (IbufferTime > 0.0f)
        IbufferTime -= dt_s;
    else
        Ibuffer = false;

    if (PressU && !Ubuffer) {
        flag = flag ? false : true;
        AppUtil::removeObjects(*this);
        AppUtil::LoadPhase(*this);
        std::cout<<"正常模式："<<flag<<std::endl;
        Ubuffer = true;
        UbufferTime = 3.0f;
    }else if (PressI && !Ibuffer) {
        m_madline->ActivateMode();
        Ibuffer = true;
        IbufferTime = 3.0f;
    }


    m_Root.Update();
    m_madline->Draw();

    // Get the current position of the main character
    glm::vec2 currentPos = m_madline->GetCoordinate();
    std::string coordinatesText = "X: " + std::to_string(currentPos.x) + " Y: " + std::to_string(currentPos.y);

    // Use the renderer to draw the text on the screen
    // Assuming m_Root.AddText() is a function that adds text to be rendered on the screen
    //std::cout << coordinatesText << std::endl;

    for (auto& child : m_PRM->GetChildren()) {
        if (auto trap = std::dynamic_pointer_cast<Trap>(child)) {
            if (m_madline->IfCollidesObject(trap)) {
                std::cout << "踩到陷阱了" << std::endl;
                AppUtil::removeObjects(*this);
                AppUtil::LoadPhase(*this);
            }
        }
        if (auto goal = std::dynamic_pointer_cast<Goal>(child)) {
            if (m_madline->IfCollidesObject(goal)) {
                AppUtil::TransitionToNextPhase(*this);
                break;
            }
        }
        if (auto bounce = std::dynamic_pointer_cast<Bounce>(child)) {
            if (m_madline->IfCollidesObject(bounce)) {
                bounce->OnCollide(*m_madline);
            }
            bounce->Update(dt_s);
        }
        if (auto box = std::dynamic_pointer_cast<WoodBox>(child)) {
            if (box->IsIntact() && m_madline->IfCollidesObject(box)) {
                box->OnCollide();               // 崩壞＋隱形牆效果移除
            }
            box->Update(dt_s);                  // 倒數計時、1.5s 後自動恢復
        }
        if (auto balloon = std::dynamic_pointer_cast<Balloon>(child)) {
            if (m_madline->IfCollidesObject(balloon)) {
                balloon->OnCollide(*m_madline);
            }
            balloon->Update(dt_s);
        }
        if (auto cloud = std::dynamic_pointer_cast<Cloud>(child)) {
            cloud->Move();
        }
    }

    m_madline->movement(m_PRM->GetWall());

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
