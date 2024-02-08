#ifndef __X2D_SYSTEMS_TIMERSERVICE_H__
#define __X2D_SYSTEMS_TIMERSERVICE_H__

#include <vector>

namespace Xeph2D
{
    class TimerService final
    {
        friend class Timer;
        friend class Runtime;
    public:
        ~TimerService() = default;
        TimerService(const TimerService& other) = delete;
        TimerService(const TimerService&& other) = delete;
        TimerService operator=(const TimerService& other) = delete;
        TimerService operator=(const TimerService&& other) = delete;
        TimerService() {}

    private:
        static TimerService& Get() { static TimerService instance; return instance; }

        static void Update();

        static void SubscribeTimer(Timer* timer);
        static void UnsubscribeTimer(Timer* timer);

        std::vector<Timer*> m_timers;
    };

}

#endif //!__X2D_SYSTEMS_TIMERSERVICE_H__
