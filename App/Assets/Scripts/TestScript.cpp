#include "TestScript.h"

using namespace Xeph2D;

void TestScript::Start()
{
	Debug::Log("Start");
}

void TestScript::Update()
{
	if (InputSystem::GetKeyHold(Key::W))
		LocalPosition().y += 1.f * Time::DeltaTime();
	if (InputSystem::GetKeyHold(Key::S))
		LocalPosition().y += -1.f * Time::DeltaTime();
	if (InputSystem::GetKeyHold(Key::D))
		LocalPosition().x += 1.f * Time::DeltaTime();
	if (InputSystem::GetKeyHold(Key::A))
		LocalPosition().x += -1.f * Time::DeltaTime();

	if (InputSystem::GetKeyDown(Key::One))
		SceneManager::LoadScene(0);
}

void TestScript::OnDestroy()
{
	Debug::Log("OnDestroy");
}
