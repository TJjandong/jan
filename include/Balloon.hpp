#ifndef BALLOON_HPP
#define BALLOON_HPP

#include "AnimatedObjects.hpp"
#include "Main_Character.hpp"

static constexpr float BalloonCollapseTime = 3.0f;   // 秒

class Balloon : public AnimatedObjects {
public:
    Balloon(const glm::vec2& coord)
    : AnimatedObjects({RESOURCE_DIR "/Image/AnimatedPicture/Balloon/balloon1.png"
                                     ,RESOURCE_DIR "/Image/AnimatedPicture/Balloon/balloon2.png"
                                     ,RESOURCE_DIR "/Image/AnimatedPicture/Balloon/balloon3.png"})
    , m_timer(0.0f)
    , m_intact(true)
    {
        SetCoordinate(coord);
        SetScale( 1.0f, 1.0f);
        SetZIndex(60);
        m_Collides = false;
    }

    void OnCollide(MainCharacter& player);

    void Update(float dt);

    private:
    float m_timer;        // 倒數計時
    bool m_intact;
};

#endif //BALLOON_HPP
