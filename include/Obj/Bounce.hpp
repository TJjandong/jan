#ifndef BOUNCE_HPP
#define BOUNCE_HPP

#include "Objects.hpp"
#include "Main_Character.hpp"
#include "WoodBox.hpp"

class Bounce : public Objects {
public:
    Bounce(const glm::vec2& coord)
    : Objects(RESOURCE_DIR"/Image/Character/tan1.png")
    , m_origImage(RESOURCE_DIR "/Image/Character/tan1.png")
    , m_activeImage(RESOURCE_DIR "/Image/Character/tan2.png")
    , m_timer(0.0f)
    , m_Pos(coord)
    {
        SetCoordinate(coord);
        SetScale( 1.0f, 1.0f);
        SetZIndex(60);
    }

    void OnCollide(MainCharacter& player);

    void Update(float dt);

    void AttachToWood(const std::shared_ptr<WoodBox>& wood) {
        m_parentWood = wood;
    }

private:
    std::weak_ptr<WoodBox> m_parentWood;        //不會和 PhaseResourceManager 的 shared_ptr<WoodBox> 產生循環引用
    std::string m_origImage;    // 原本的空閒貼圖
    std::string m_activeImage;  // 被踩的貼圖
    float       m_timer;        // 倒數計時
    glm::vec2   m_Pos;
    const float BounceTransTime = 0.5f;   // 秒
};

#endif
