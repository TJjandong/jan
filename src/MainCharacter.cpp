#include "Characters.hpp"
#include <string>
#include <iostream>
#include "util/Input.hpp"
#include "Main_Character.hpp"

void MainCharacter::movement() {
    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        if (velocity_x < 4.0f) {
            velocity_x += 2.0f;
        }else {
            velocity_x = 4.0f;
        }
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        if (velocity_x > -4.0f) {
            velocity_x -= 2.0f;
        }else {
            velocity_x = -4.0f;
        }
    }

    // Apply friction if no key is pressed (gradually decelerate the character)
    if (!Util::Input::IsKeyPressed(Util::Keycode::RIGHT) && !Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        if (velocity_x > 0) {
            velocity_x -= 0.1f;  // Gradual deceleration to the right
            if (velocity_x < 0) velocity_x = 0;  // Stop at zero
        } else if (velocity_x < 0) {
            velocity_x += 0.1f;  // Gradual deceleration to the left
            if (velocity_x > 0) velocity_x = 0;  // Stop at zero
        }
    }

    /*if (!Util::Input::IsKeyPressed(Util::Keycode::UP) && !Util::Input::IsKeyPressed(Util::Keycode::DOWN)) {
        if (velocity_y > 0) {
            velocity_y -= 0.1f;  // Gradual deceleration upwards
            if (velocity_y < 0) velocity_y = 0;  // Stop at zero
        } else if (velocity_y < 0) {
            velocity_y += 0.1f;  // Gradual deceleration downwards
            if (velocity_y > 0) velocity_y = 0;  // Stop at zero
        }
    }       之後要改成重力*/

    glm::vec2 currentPos = GetPosition(); // 獲取當前位置
    currentPos.x += velocity_x; // 調整這個數值來控制移動速度
    currentPos.y += velocity_y; // 調整這個數值來控制移動速度
    SetPosition(currentPos); // 更新的位置
}
