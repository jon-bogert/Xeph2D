#include "Xeph2D/Systems/Time.h"

using namespace Xeph2D;

Time& Time::Get()
{
	static Time instance;
	return instance;
}

float Xeph2D::Time::DeltaTime()
{
	return Get().m_delta * Get().m_timescale;
}

float Xeph2D::Time::UnscaledDeltaTime()
{
	return Get().m_delta;
}

float Xeph2D::Time::GetTimeScale()
{
	return Get().m_timescale;
}

void Xeph2D::Time::SetTimeScale(float timescale)
{
	Get().m_timescale = timescale;
}

uint32_t Xeph2D::Time::FPS()
{
	return (Get().m_delta <= 0.f) ? 0.f : 1.f / Get().m_delta;
}

void Xeph2D::Time::Update()
{
	Get().m_delta = Get().m_timer.restart().asSeconds();
}
