#pragma once

#include <SFML.hpp>

namespace Xeph2D
{
	class Time final
	{
	public:
		~Time() = default;
		Time(const Time& other) = delete;
		Time(const Time&& other) = delete;
		Time operator=(const Time& other) = delete;
		Time operator=(const Time&& other) = delete;


		static float DeltaTime();
		static float UnscaledDeltaTime();
		static float GetTimeScale();
		static void SetTimeScale(float timescale);
		static uint32_t FPS();

	private:
		Time() {}
		static Time& Get();

		friend class Runtime;
		static void Update();

		float m_delta = 0.f;
		float m_timescale = 1.f;
		sf::Clock m_timer = {};
	};
}