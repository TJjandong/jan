#ifndef VICUI_HPP
#define VICUI_HPP

#include "Obj/Objects.hpp"
#include "Util/Renderer.hpp"
#include "Util/Logger.hpp"

class VictoryUI : public Objects{
public:
    // 傳入渲染根節點，用於 AddChild/RemoveChild
    VictoryUI(Util::Renderer& root)
    : Objects(RESOURCE_DIR "/Victory/VicMenu.png")
    , m_root(root) {
        SetZIndex(100);
    }

    void Show(float timeSec, int deaths);

    void Hide();

    void CreateNumberSprites(Util::Renderer& root,
        const std::string& str,
        float startX,
        float y,
        std::vector<std::shared_ptr<Objects>>& items
        );

private:
    Util::Renderer& m_root;
    std::vector<std::shared_ptr<Objects>> m_items;
};


#endif //VICUI_HPP
