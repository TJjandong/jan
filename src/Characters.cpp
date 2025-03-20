#include "Characters.hpp"

#include <iostream>

#include "Util/Image.hpp"

Characters::Characters(const std::string& ImagePath) {
    SetImage(ImagePath);
    ResetPosition();
}

void Characters::SetImage(const std::string& ImagePath) {
    m_ImagePath = ImagePath;

    // 檢查圖片加載是否成功
    try {
        m_Drawable = std::make_shared<Util::Image>(m_ImagePath);
        if (!m_Drawable) {
            std::cerr << "Failed to load image: " << m_ImagePath << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading image: " << e.what() << std::endl;
    }
}

void Characters::SetScale(float scaleX, float scaleY) {
    m_Transform.scale = {scaleX, scaleY};
}

bool Characters::IfCollides(const std::shared_ptr<Characters>& other) const {
    // Get the position and scale of this character
    glm::vec2 thisPos = m_Transform.translation;
    float thisWidth = m_Transform.scale.x;  // Assuming scale.x represents width
    float thisHeight = m_Transform.scale.y; // Assuming scale.y represents height

    // Get the position and scale of the other character
    glm::vec2 otherPos = other->GetPosition();
    float otherWidth = other->m_Transform.scale.x;  // Assuming scale.x represents width
    float otherHeight = other->m_Transform.scale.y; // Assuming scale.y represents height

    // Check for overlap on both axes (X and Y)
    bool overlapX = thisPos.x < otherPos.x + otherWidth && thisPos.x + thisWidth > otherPos.x;
    bool overlapY = thisPos.y < otherPos.y + otherHeight && thisPos.y + thisHeight > otherPos.y;

    return overlapX && overlapY; // If both X and Y overlap, a collision occurred
}
