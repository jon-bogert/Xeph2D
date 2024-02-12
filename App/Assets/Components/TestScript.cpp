#include "TestScript.h"

using namespace Xeph2D;

void TestScript::Serializables()
{
	SERIALIZE_COLOR(m_colors[0]);
	SERIALIZE_COLOR(m_colors[1]);
	SERIALIZE_BOOL(m_isMain);
	SERIALIZE_REF(m_spriteRenderer);
}

void TestScript::OnEditorStart()
{
	m_spriteRenderer = FindObjectOfType<SpriteRenderer>();
	if (m_spriteRenderer == nullptr)
	{
		Debug::Log("SpriteRenderer component was null");
	}
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
		m_spriteRenderer->LocalPosition().y += 5.f * Time::DeltaTime();
	if (InputSystem::GetKeyHold(Key::S))
		m_spriteRenderer->LocalPosition().y += -5.f * Time::DeltaTime();
	if (InputSystem::GetKeyHold(Key::D))
		m_spriteRenderer->LocalPosition().x += 5.f * Time::DeltaTime();
	if (InputSystem::GetKeyHold(Key::A))
		m_spriteRenderer->LocalPosition().x += -5.f * Time::DeltaTime();

	if (InputSystem::GetKeyDown(Key::One))
		SceneManager::LoadScene(0);
}

void TestScript::OnDestroy()
{
	Debug::Log("OnDestroy");
}
