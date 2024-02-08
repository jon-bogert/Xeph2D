#include "Xeph2D/Timer.h"

#include "Xeph2D/Systems/TimerService.h"
#include "Xeph2D/Systems/Time.h"

Xeph2D::Timer::Timer()
{
	TimerService::SubscribeTimer(this);
}

Xeph2D::Timer::Timer(float durationSeconds)
{
	m_lengthSeconds = durationSeconds;
	TimerService::SubscribeTimer(this);
}

Xeph2D::Timer::~Timer()
{
	TimerService::UnsubscribeTimer(this);
}

bool Xeph2D::Timer::Expired() const
{
	return m_isExpired;
}

bool Xeph2D::Timer::ExpiredThisFrame() const
{
	return (m_isExpired && !m_wasExpired);
}

void Xeph2D::Timer::Reset()
{
	m_isRunning = false;
	m_currentSeconds = m_lengthSeconds;
}

void Xeph2D::Timer::Restart()
{
	m_isRunning = true;
	m_isExpired = false;
	m_wasExpired = false;
	m_currentSeconds = m_lengthSeconds;
}

void Xeph2D::Timer::Start(const float length)
{
	if (length > 0)
	{
		m_lengthSeconds = length;
		m_currentSeconds = length;
	}
	m_isRunning = true;
	m_isExpired = false;
	m_wasExpired = false;
}

void Xeph2D::Timer::Pause()
{
	m_isRunning = false;
}

void Xeph2D::Timer::Update()
{
	if (!m_isRunning)
		return;
	if (m_isExpired && !m_isLooping)
	{
		if (!m_wasExpired)
			m_wasExpired = true;
		return;
	}

	float time = (m_isUnscaled) ? Time::UnscaledDeltaTime() : Time::DeltaTime();
	m_currentSeconds -= time;

	m_wasExpired = m_isExpired;
	m_isExpired = (m_currentSeconds <= 0.f);
	
	if (m_isExpired && m_isLooping)
		time += m_lengthSeconds;
}
