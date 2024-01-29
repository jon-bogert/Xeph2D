#ifndef __XEPH2D_EVENTS_H__
#define __XEPH2D_EVENTS_H__

#include <functional>

#define Xeph2D_VoidEventCallback(function) static_cast<void*>(this), std::bind(&function, this)
#define Xeph2D_VoidEventCallbackPtr(function, ptr) static_cast<void*>(ptr), std::bind(&function, ptr)

namespace Xeph2D
{
    class VoidEvent
    {
    private:
        struct EventEntry
        {
            void* object;
            std::function<void(void)> function;
        };

        std::vector<EventEntry> events;

    public:
        void Subscribe(void* object, std::function<void(void)> function)
        {
            events.push_back({ object, function });
        }

        void UnsubscribeAll(void* object)
        {
            for (auto& iter = events.begin(); iter != events.end();)
            {
                if (iter->object == object)
                    iter = events.erase(iter);
                else
                    ++iter;
            }
        }

        void Invoke() {
            for (const auto& event : events)
            {
                event.function();
            }
        }
    };
}

#endif //!__XEPH2D_EVENTS_H__