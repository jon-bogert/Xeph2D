#include "TestScript.h"

using namespace Xeph2D;

void TestScript::Serializables()
{
	SERIALIZE_FLOAT(m_frameRate);
}

void TestScript::Awake()
{
	m_frameTime = 1.f / m_frameRate;
}

void TestScript::Start()
{
	m_spritesheet = GetComponent<SpritesheetRenderer>();
	m_timer.SetLengthSeconds(m_frameTime);
	m_timer.SetLooping(true);
	m_timer.Start();

	UpdateSpritesheet();
}

void TestScript::Update()
{
	if (InputSystem::GetKeyHold(Key::W))
		LocalPosition().y += 5.f * Time::DeltaTime();
	if (InputSystem::GetKeyHold(Key::S))
		LocalPosition().y += -5.f * Time::DeltaTime();
	if (InputSystem::GetKeyHold(Key::D))
		LocalPosition().x += 5.f * Time::DeltaTime();
	if (InputSystem::GetKeyHold(Key::A))
		LocalPosition().x += -5.f * Time::DeltaTime();

	AnimationSwitch();

	if (m_timer.ExpiredThisFrame())
		UpdateSpritesheet();
}

void TestScript::AnimationSwitch()
{
	bool doSwitch = false;
	if (InputSystem::GetKeyHold(Key::One))
	{
		m_currAnimation = Animation::Idle;
		doSwitch = true;
	}
	else if (InputSystem::GetKeyHold(Key::Two))
	{
		m_currAnimation = Animation::Run;
		doSwitch = true;
	}
	else if (InputSystem::GetKeyHold(Key::Three))
	{
		m_currAnimation = Animation::Shoot;
		doSwitch = true;
	}
	else if (InputSystem::GetKeyHold(Key::Four))
	{
		m_currAnimation = Animation::Climb;
		doSwitch = true;
	}
	else if (InputSystem::GetKeyHold(Key::Five))
	{
		m_currAnimation = Animation::Roll;
		doSwitch = true;
	}

	if (doSwitch)
	{
		m_timer.Restart();
		m_currFrame = -1; // will be incremented in UpdateSpritesheet
		UpdateSpritesheet();
	}
}

void TestScript::UpdateSpritesheet()
{
	int row = 0;
	switch (m_currAnimation)
	{
	case Animation::Idle:
		row = 1;
		m_currFrame = (m_currFrame + 1) % 4;
		break;
	case Animation::Run:
		row = 5;
		m_currFrame = (m_currFrame + 1) % 10;
		break;
	case Animation::Shoot:
		row = 0;
		m_currFrame = (m_currFrame + 1) % 5;
		break;
	case Animation::Climb:
		row = 3;
		m_currFrame = (m_currFrame + 1) % 2;
		break;
	case Animation::Roll:
		row = 2;
		m_currFrame = (m_currFrame + 1) % 5;
		break;
	default:
		Debug::LogErr("Unimplemented Animation type");
		break;
	}

	m_spritesheet->SetCell({ (float)m_currFrame, (float)row });
}
