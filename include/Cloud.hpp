#ifndef CLOUD_HPP
#define CLOUD_HPP

#include "Objects.hpp"
#include "Main_Character.hpp"

static constexpr glm::vec2 MoveLeft = {-2.0f, 0.0f};
static constexpr glm::vec2 MoveRight = {2.0f, 0.0f};

class Cloud : public Objects {
public:
    enum class Orientation { Left , Right };
    Cloud(const glm::vec2& coord, Orientation orientation)
    : Objects(RESOURCE_DIR"/Image/Character/cloud.png")
    , m_orientation(orientation)
    {
        SetCoordinate(coord - glm::vec2{0.0f, 24.0f});
        SetScale(2.0f, 0.1f);
        SetZIndex(100);
    }

    void ReturnMap();

    void Move();

    bool IsRight() {
        return m_orientation == Orientation::Right;
    }

private:
    Orientation m_orientation;
};

#endif //CLOUD_HPP
