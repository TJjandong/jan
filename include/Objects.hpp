#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <utility>
#include <iostream>
#include <string>

#include "Util/Image.hpp"
#include "Util/GameObject.hpp"

class Objects : public Util::GameObject {
public:
    explicit Objects(const std::string& ImagePath);

    Objects(const Objects&) = delete;

    Objects(Objects&&) = delete;

    Objects& operator=(const Objects&) = delete;

    Objects& operator=(Objects&&) = delete;

    [[nodiscard]] const std::string& GetImagePath() const { return m_ImagePath; }

    [[nodiscard]] const glm::vec2& GetCoordinate() const { return m_Transform.translation; }

    const glm::vec2& GetPosition() const { return m_Position; }

    [[nodiscard]] bool GetVisibility() const { return m_Visible; }

    void SetImage(const std::string& ImagePath);

    void SetCoordinate(const glm::vec2& Coordinate);

    // TODO: Add and implement more methods and properties as needed to finish Giraffe Adventure.
    void SetScale(float scaleX, float scaleY);



private:
    void ResetPosition() { m_Transform.translation = {0, 0}; }

    std::string m_ImagePath;
    glm::vec2 m_Position{ (m_Transform.translation.x + 384) / 6.0f , (m_Transform.translation.y + 384) / 6.0f };  // 存儲位置數據
};

#endif
