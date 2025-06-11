// AppUtil.hpp
#ifndef APPUTIL_HPP
#define APPUTIL_HPP

class App;    // declare

namespace AppUtil {
    // 僅聲明函数簽名，不訪問私有成员
    void removeObjects(App& app);
    void LoadPhase(App& app);
    void TransitionToNextPhase(App& app);
    void VictorySetup(App& app);
    void TimeSetup(float time);
    void numPrint(int num);
}

#endif // APPUTIL_HPP
