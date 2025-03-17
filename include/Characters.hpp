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

    [[nodiscard]] const glm::vec2& GetPosition() const { return m_Transform.translation; }

    [[nodiscard]] bool GetVisibility() const { return m_Visible; }

    void SetImage(const std::string& ImagePath);

    void SetPosition(const glm::vec2& Position) { m_Transform.translation = Position; }

    // TODO: Implement the collision detection
    [[nodiscard]] bool IfCollides(const std::shared_ptr<Characters>& other) const {
        (void) other;
        return true;
    }

    // TODO: Add and implement more methods and properties as needed to finish Giraffe Adventure.
    void SetScale(float scaleX, float scaleY);



private:
    void ResetPosition() { m_Transform.translation = {0, 0}; }

    std::string m_ImagePath;
};

#endif
