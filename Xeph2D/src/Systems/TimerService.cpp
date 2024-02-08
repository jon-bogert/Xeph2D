#include "Xeph2D/Systems/TimerService.h"
#include "Xeph2D/Timer.h"
#include "Xeph2D/Systems/Debug.h"

void Xeph2D::TimerService::Update()
{
	for (Timer* timer : Get().m_timers)
	{
		timer->Update();
	}
}

void Xeph2D::TimerService::SubscribeTimer(Timer* timer)
{
	Get().m_timers.push_back(timer);
}

void Xeph2D::TimerService::UnsubscribeTimer(Timer* timer)
{
	//Destructor already called
	if (Get().m_timers.empty())
		return;

	auto iter = std::find_if(Get().m_timers.begin(), Get().m_timers.end(), [=](const Timer* t) {return t == timer; });
	if (iter == Get().m_timers.end())
	{
		Debug::LogErr("TimerService::Unsubscribe -> could not find timer to unsubscribe");
		return;
	}

	Get().m_timers.erase(iter);
}
