#include "TestScript.h"

using namespace Xeph2D;

void TestScript::Serializables()
{
	
}

void TestScript::Start()
{

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

	
}
