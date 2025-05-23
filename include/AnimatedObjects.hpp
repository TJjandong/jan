#ifndef ANIMATED_OBJECTS_HPP
#define ANIMATED_OBJECTS_HPP

#include <vector>
#include <string>

#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"


class AnimatedObjects : public Util::GameObject {

public:
    explicit AnimatedObjects(const std::vector<std::string>& AnimationPaths);

    [[nodiscard]] bool IsLooping() const {
        return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetLooping();
    }

    [[nodiscard]] bool IsPlaying() const {
        return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetState() == Util::Animation::State::PLAY;
    }

    void SetLooping(bool looping) {
        auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        temp->SetLooping(looping);
    }

    void SetCoordinate(const glm::vec2& Coordinate);

    void SetScale(float scaleX, float scaleY);

    const glm::vec2& GetCoordinate() const{ return m_Transform.translation; }

    [[nodiscard]] bool IfAnimationEnds() const;

    void SetFrame(int frame) {
        m_frames = frame;
    }

    bool IsCollidable() {
        return m_Collides;
    };

private:
    int m_frames = 500;
protected:
    bool m_Collides = true;
};

#endif //ANIMATED_CHARACTER_HPP
