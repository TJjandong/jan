#ifndef APPUTIL_HPP
#define APPUTIL_HPP

#include "Objects.hpp"
#include "BackgroundImage.hpp"

[[nodiscard]]inline bool isWall(const Objects& character) {
    auto position = character.GetPosition();
    return position.y > -90 && position.y < 93 && position.x > 50 && position.x < 233;
}

/*[[nodiscard]]inline bool AreAllDoorsOpen(const std::vector<std::shared_ptr<Characters>>& doors) {
    return std::all_of(doors.begin(), doors.end(), [](const auto& door) { return door->GetImagePath() ==
    GA_RESOURCE_DIR"/Image/Character/door_open.png"; });
}
*/
#endif //APP_UTIL_HPP


