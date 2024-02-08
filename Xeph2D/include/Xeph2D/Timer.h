#ifndef __X2D_TIMER_H__
#define __X2D_TIMER_H__

namespace Xeph2D
{
	class Timer
	{
	public:
		Timer();
		Timer(float durationSeconds);
		virtual ~Timer();

		bool Expired() const;
		bool ExpiredThisFrame() const;

		void Reset();
		void Restart();
		void Start(const float length = -1);
		void Pause();

		void SetLengthSeconds(const float length) { m_lengthSeconds = length; }
		float LengthSeconds() const { return m_lengthSeconds; }

		void SetIsUnscaled(const bool isUnscaled) { m_isUnscaled = isUnscaled; }
		bool IsUnscaled() const { return m_isUnscaled; }

		bool IsRunning() const { return m_isRunning; }

		void SetLooping(const bool isLooping) { m_isLooping = isLooping; }
		bool IsLoop() const { return m_isLooping; }

	protected:
		float m_lengthSeconds = 0.f;
		float m_currentSeconds = 0.f;

		bool m_isRunning = false;
		bool m_isUnscaled = false;

		bool m_isExpired = false;
		bool m_wasExpired = false;

		bool m_isLooping = false;

		friend class TimerService;
		void Update();
	};
}

#endif //!__X2D_TIMER_H__
