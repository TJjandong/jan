#ifndef CLOUD_HPP
#define CLOUD_HPP

#include "Objects.hpp"
#include "Main_Character.hpp"

static constexpr glm::vec2 MoveLeft = {-2.0f, 0.0f};
static constexpr glm::vec2 MoveRight = {2.0f, 0.0f};

class Cloud : public Objects {
public:
    enum class Direction { Left , Right };
    Cloud(const glm::vec2& coord, Direction direction)
    : Objects(RESOURCE_DIR"/Image/Character/cloud.png")
    , m_direction(direction)
    {
        SetCoordinate(coord - glm::vec2{0.0f, 24.0f});
        SetScale(2.0f, 0.1f);
        SetZIndex(90);
    }

    void ReturnMap();

    void Move();

    bool IsRight() {
        return m_direction == Direction::Right;
    }

    glm::vec2 GetDelta() const{ return m_delta; }

private:
    Direction m_direction;
    glm::vec2 m_delta;
};

#endif //CLOUD_HPP
