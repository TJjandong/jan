// AppUtil.hpp
#ifndef APPUTIL_HPP
#define APPUTIL_HPP

class App;    // declare

namespace AppUtil {
    // 僅聲明函数簽名，不訪問私有成员
    void LoadPhase(App& app);
    void TransitionToNextPhase(App& app);
}

#endif // APPUTIL_HPP
