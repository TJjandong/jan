#ifndef GIRAFFE_HPP
#define GIRAFFE_HPP

#include <utility>

#include <string>
#include "Util/GameObject.hpp"

class Characters : public Util::GameObject {
public:
    explicit Characters(const std::string& ImagePath);

    Characters(const Characters&) = delete;

    Characters(Characters&&) = delete;

    Characters& operator=(const Characters&) = delete;

    Characters& operator=(Characters&&) = delete;

    [[nodiscard]] const std::string& GetImagePath() const { return m_ImagePath; }

    [[nodiscard]] const glm::vec2& GetCoordinate() const { return m_Transform.translation; }

    const glm::vec2& GetPosition() const { return m_Position; }

    [[nodiscard]] bool GetVisibility() const { return m_Visible; }

    void SetImage(const std::string& ImagePath);

    void SetCoordinate(const glm::vec2& Coordinate);

    // TODO: Implement the collision detection
    bool IfCollides(const std::shared_ptr<Characters>& other) const;

    // TODO: Add and implement more methods and properties as needed to finish Giraffe Adventure.
    void SetScale(float scaleX, float scaleY);



private:
    void ResetPosition() { m_Transform.translation = {0, 0}; }

    std::string m_ImagePath;
    glm::vec2 m_Position{ (m_Transform.translation.x + 350) / 5.0f , (m_Transform.translation.y + 350) / 5.0f };  // 存儲位置數據
};

#endif
