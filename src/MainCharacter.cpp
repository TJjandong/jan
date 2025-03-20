#include "Characters.hpp"
#include <string>
#include <iostream>
#include "util/Input.hpp"
#include "Main_Character.hpp"

void MainCharacter::movement() {
    const float max_speed = 5.0f;  // 設定最大速度
    const float acceleration = 4.0f;  // 設定加速度
    const float friction = 4.0f;  // 設定摩擦力（減速值）

    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        velocity_x += acceleration;
        if (velocity_x > max_speed) velocity_x = max_speed;  // 限制最大速度
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        velocity_x -= acceleration;
        if (velocity_x < -max_speed) velocity_x = -max_speed;  // 限制最小速度
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::UP)) {
        velocity_y += acceleration;
        if (velocity_y > max_speed) velocity_y = max_speed;
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::DOWN)) {
        velocity_y -= acceleration;
        if (velocity_y < -max_speed) velocity_y = -max_speed;
    }

    // Apply friction if no key is pressed (gradually decelerate the character)
    if (!Util::Input::IsKeyPressed(Util::Keycode::RIGHT) && !Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        if (velocity_x > 0) {
            velocity_x -= friction;  // Gradual deceleration to the right
            if (velocity_x < 0) velocity_x = 0;  // Stop at zero
        } else if (velocity_x < 0) {
            velocity_x += friction;  // Gradual deceleration to the left
            if (velocity_x > 0) velocity_x = 0;  // Stop at zero
        }
    }

    if (!Util::Input::IsKeyPressed(Util::Keycode::UP) && !Util::Input::IsKeyPressed(Util::Keycode::DOWN)) {
        if (velocity_y > 0) {
            velocity_y -= friction;  // Gradual deceleration upwards
            if (velocity_y < 0) velocity_y = 0;  // Stop at zero
        } else if (velocity_y < 0) {
            velocity_y += friction;  // Gradual deceleration downwards
            if (velocity_y > 0) velocity_y = 0;  // Stop at zero
        }
    }

    glm::vec2 currentPos = GetCoordinate(); // 獲取當前位置
    currentPos.x += velocity_x; // 調整這個數值來控制移動速度
    currentPos.y += velocity_y; // 調整這個數值來控制移動速度
    SetCoordinate(currentPos); // 更新的位置
}
