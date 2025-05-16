#include "Objects.hpp"

Objects::Objects(const std::string& ImagePath) {
    SetImage(ImagePath);
    ResetPosition();
}

void Objects::SetImage(const std::string& ImagePath) {
    m_ImagePath = ImagePath;

    if (ImagePath.empty()) {
        // 若路徑為空，不載入圖片，並將 m_Drawable 設為 nullptr
        m_Drawable = nullptr;
        return;
    }

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

void Objects::SetScale(float scaleX, float scaleY) {
    m_Transform.scale = {scaleX, scaleY};
}

void Objects::SetCoordinate(const glm::vec2& Coordinate) {
    m_Transform.translation = Coordinate;
    m_Position.x = (m_Transform.translation.x + 384) / 6.0f;
    m_Position.y = (m_Transform.translation.y + 384) / 6.0f;
}
