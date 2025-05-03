#ifndef INVISIBLE_WALL_HPP
#define INVISIBLE_WALL_HPP

#include "Objects.hpp"

// InvisibleWall 繼承自 Characters，不顯示圖片，只用來作為碰撞用的隱形牆
class InvisibleWall : public Objects {
public:
    // 由於不需要顯示圖片，這裡傳入空字串或透明圖檔的路徑
    InvisibleWall(const glm::vec2& coordinate)//C:/Users/jan20/jan/Resources/Image/BackgroundImg/pixil-frame-0.png
        : Objects(""), m_WallCoordinate(coordinate)
    {
        SetCoordinate(coordinate);  // 設定牆壁位置
        SetZIndex(20);  // 設定較低的 Z-Index
    }


private:
    glm::vec2 m_WallCoordinate;
};

#endif
