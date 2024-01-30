#include "TestScript.h"

using namespace Xeph2D;

void TestScript::Start()
{
	Debug::Log("Start");
}

void TestScript::Update()
{
	if (InputSystem::GetKeyDown(Key::One))
	{
		SceneManager::LoadScene(0);
	}
	if (InputSystem::GetKeyDown(Key::Two))
	{
		SceneManager::LoadScene(1);
	}
}

void TestScript::OnDestroy()
{
	Debug::Log("OnDestroy");
}
