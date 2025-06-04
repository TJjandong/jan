#include "Obj/AnimatedObjects.hpp"


AnimatedObjects::AnimatedObjects(const std::vector<std::string>& AnimationPaths) {
    m_Drawable = std::make_shared<Util::Animation>(AnimationPaths, true, m_frames, true, 0);
}

void AnimatedObjects::SetScale(float scaleX, float scaleY) {
    m_Transform.scale = {scaleX, scaleY};
}

void AnimatedObjects::SetCoordinate(const glm::vec2& Coordinate) {
    m_Transform.translation = Coordinate;
}

bool AnimatedObjects::IfAnimationEnds() const {
    auto animation = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    return animation->GetCurrentFrameIndex() == animation->GetFrameCount() - 1;
}
