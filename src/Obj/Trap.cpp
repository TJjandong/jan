// Trap.cpp
#include "Obj/Trap.hpp"

void Trap::OnCollide(MainCharacter& player) {
    // 依需求實作死亡機制，例如直接重置角色位置：
    std::cout << "踩到陷阱了" << std::endl;
    player.KillInstant();
}
