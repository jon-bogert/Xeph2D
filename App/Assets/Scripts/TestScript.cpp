#include "TestScript.h"

using namespace Xeph2D;

void TestScript::Serializables()
{
	SERIALIZE_COLOR(m_colors[0]);
	SERIALIZE_COLOR(m_colors[1]);
}

void TestScript::Start()
{
	Debug::Log("Start");
	m_spriteRenderer = FindObjectOfType<SpriteRenderer>();
	if (m_spriteRenderer == nullptr)
	{
		Debug::Log("SpriteRenderer component was null");
	}
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

	if (InputSystem::GetKeyDown(Key::One))
		SceneManager::LoadScene(0);

	if (InputSystem::GetKeyDown(Key::Space))
	{
		m_colorIndex = (m_colorIndex + 1) % 2;
		m_spriteRenderer->SetColor(m_colors[m_colorIndex]);
		//m_spriteRenderer->SetEnabled(!m_spriteRenderer->Enabled());
		//gameObject->SetIsActive(!gameObject->IsActive());
	}
}

void TestScript::OnDestroy()
{
	Debug::Log("OnDestroy");
}
