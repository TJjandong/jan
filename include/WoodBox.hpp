#ifndef WOODBOX_HPP
#define WOODBOX_HPP

#include "Objects.hpp"
#include "Main_Character.hpp"
#include "Util/Time.hpp"

static constexpr float WoodCollapseTime = 3.0f;   // 秒

class WoodBox : public Objects {
public:
    WoodBox(const glm::vec2& coord)
    : Objects(RESOURCE_DIR"/Image/Character/box1.png")
    , m_Image1(RESOURCE_DIR"/Image/Character/box1.png")
    , m_Image2(RESOURCE_DIR"/Image/Character/box2.png")
    , m_Image3(RESOURCE_DIR"/Image/Character/box3.png")
    , m_timer(0.0f)
    , m_intact(true)
    {
        SetCoordinate(coord);
        SetScale( 1.0f, 1.0f);
        SetZIndex(70);
    }

    void OnCollide();

    void Update(float dt);

    bool IsIntact() const { return m_intact; }

private:
    std::string m_Image1;       // 原本的空閒貼圖
    std::string m_Image2;       // 被碰到的貼圖
    std::string m_Image3;       // 分解的貼圖
    float       m_timer;        // 倒數計時
    bool        m_intact;
};


#endif //WOODBOX_HPP
