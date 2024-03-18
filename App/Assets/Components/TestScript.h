#pragma once

#include <Xeph2D.h>

class TestScript : public Xeph2D::Component
{
	XEPH2D_REG_COMP(0xa45ff78b);
private:
	void Serializables() override;

	void Awake() override;
	void Start() override;
	void Update() override;

	void AnimationSwitch();
	void UpdateSpritesheet();

	Xeph2D::Ref<Xeph2D::SpritesheetRenderer> m_spritesheet;
	Xeph2D::Timer m_timer;
	float m_frameRate = 12.f;
	float m_frameTime = 1.f / 12.f;
	int m_currFrame = -1;

	enum class Animation
	{
		Shoot,
		Idle,
		Roll,
		Climb,
		Run
	};

	Animation m_currAnimation = Animation::Idle;
};