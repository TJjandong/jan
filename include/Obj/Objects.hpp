#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <utility>
#include <iostream>
#include <string>

#include "Util/Image.hpp"
#include "Util/GameObject.hpp"

//加上 inline：告訴編譯器「這個函式可以在多個翻譯單元都定義，連結器會幫你合併成一個實體，不會出現多重定義錯誤」
inline bool operator>(const glm::vec2& A, const glm::vec2& B){
    return A.x > B.x && A.y > B.y;
}
inline bool operator<(const glm::vec2& A, const glm::vec2& B){
    return A.x < B.x && A.y < B.y;
}

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

    bool IsCollidable() {
        return m_Collides;
    };

private:
    void ResetPosition() { m_Transform.translation = {0, 0}; }

    std::string m_ImagePath;
    glm::vec2 m_Position{ (m_Transform.translation.x + 384) / 6.0f , (m_Transform.translation.y + 384) / 6.0f };  // 存儲位置數據

protected:
    bool m_Collides = true;
};

#endif
