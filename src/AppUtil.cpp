// AppUtil.cpp  新增
#include "AppUtil.hpp"
#include "App.hpp"
#include "Util/Logger.hpp"

namespace AppUtil {

    void LoadPhase(App& app) {
        // 1) 設定關卡編號
        int phaseIndex = static_cast<int>(app.m_Phase);

        // 2) 讓 ResourceManager 讀取對應的地圖／陷阱／目標
        app.m_PRM->SetBoundary(phaseIndex);  // 讀取 0.txt, 1.txt ... :contentReference[oaicite:1]{index=1}:contentReference[oaicite:2]{index=2}

        // 3) 清空 Renderer，把角色和新的所有物件加回去
        app.m_Root.RemoveChild(app.m_madline);
        for (auto& obj : app.m_PRM->GetChildren()) {
            app.m_Root.RemoveChild(obj);
        }

        app.m_Root.AddChild(app.m_madline);
        app.m_Root.AddChildren(app.m_PRM->GetChildren());  // 背景、牆、陷阱、目標 :contentReference[oaicite:3]{index=3}
        // 4) 重設角色重生點（可選）：每關可以有不同的 spawn point
        switch (phaseIndex) {
            case 1:
                app.m_madline->SetSpawnPoint({-300.0f, -250.0f});
                break;
            default:
                app.m_madline->SetSpawnPoint({-300.0f, -150.0f});
                break;
        }
    }

    void TransitionToNextPhase(App& app) {
        // 1) 更新 App::Phase enum
        using Phase = App::Phase;
        int next = static_cast<int>(app.m_Phase) + 1;
        app.m_Phase = static_cast<Phase>(next);

        // 2) 讓 PhaseResourceManager 換背景圖
        app.m_PRM->NextPhase(next);            // 背景圖由 phase0.png→ … :contentReference[oaicite:4]{index=4}:contentReference[oaicite:5]{index=5}


        // 3) 再次呼叫 LoadPhase 載入新的地圖、道具、牆壁、陷阱、目標
        LoadPhase(app);
    }

} // namespace AppUtil
