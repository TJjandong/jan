// AppUtil.cpp  新增
#include "AppUtil.hpp"
#include "App.hpp"
#include "Util/Logger.hpp"

namespace AppUtil {

    void removeObjects(App& app) {
        app.m_Root.RemoveChild(app.m_madline);
        auto oldChildren = app.m_PRM->GetChildren();
        for (auto& obj : oldChildren) {
            app.m_Root.RemoveChild(obj);
        }
    }

    void LoadPhase(App& app) {
        // 設定關卡編號
        int phaseIndex = static_cast<int>(app.m_Phase);

        // 讓 ResourceManager 讀取對應的地圖／陷阱／目標
        app.m_PRM->SetBoundary(phaseIndex, app.flag);  // 讀取 0.txt, 1.txt ... :contentReference[oaicite:1]{index=1}:contentReference[oaicite:2]{index=2}
        app.m_Root.AddChild(app.m_madline);
        app.m_Root.AddChildren(app.m_PRM->GetChildren());  // 背景、牆、陷阱、目標 :contentReference[oaicite:3]{index=3}

        switch (phaseIndex) {
            case App::Phase01:
                app.m_madline->SetSpawnPoint({-300.0f, -300.0f});
                break;
            case app.Phase02:
                app.m_madline->SetSpawnPoint({-300.0f, -250.0f});
            break;
            case app.Phase04:
                app.m_madline->SetSpawnPoint({-300.0f, -150.0f});
            break;
            case app.Phase05:
                app.m_madline->SetSpawnPoint({-300.0f, -150.0f});
            break;
            case app.Phase06:
                app.m_madline->SetSpawnPoint({-250.0f, -300.0f});
            break;
            case app.Phase07:
                app.m_madline->SetSpawnPoint({-300.0f, -300.0f});
            break;
            case app.Phase08:
                app.m_madline->SetSpawnPoint({-250.0f, -200.0f});
            break;
            case app.Phase09:
                app.m_madline->SetSpawnPoint({-300.0f, -250.0f});
            break;
            case app.Phase10:
                app.m_madline->SetSpawnPoint({-300.0f, -300.0f});
            break;
            default:
                app.m_madline->SetSpawnPoint({-300.0f, -200.0f});
                break;
        }
    }

    void TransitionToNextPhase(App& app) {
        // 更新 App::Phase enum
        using Phase = App::Phase;
        int next = static_cast<int>(app.m_Phase) + 1;
        app.m_Phase = static_cast<Phase>(next);

        // 清空 Renderer，把角色和新的所有物件加回去
        removeObjects(app);

        // 讓 PhaseResourceManager 換背景圖
        app.m_PRM->NextPhase(next);            // 背景圖由 phase0.png→ … :contentReference[oaicite:4]{index=4}:contentReference[oaicite:5]{index=5}


        // 再次呼叫 LoadPhase 載入新的地圖、道具、牆壁、陷阱、目標
        LoadPhase(app);
    }

} // namespace AppUtil
