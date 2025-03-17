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


