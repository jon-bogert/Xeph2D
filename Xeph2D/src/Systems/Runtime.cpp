#include "Xeph2D/Systems/Runtime.h"

#include "Xeph2D/Systems/AssetManager.h"
#include "Xeph2D/Systems/SceneManager.h"
#include "Xeph2D/Scene.h"
#include "Xeph2D.h"

using namespace Xeph2D;

#ifdef _EDITOR

void Xeph2D::Runtime::Initialize()
{
}

void Xeph2D::Runtime::Initialize(
	std::function<std::unordered_map<uint32_t, std::string>(void)> namingCallback,
	std::function<void(std::shared_ptr<Component>& ptr, uint32_t compID)> populateCallback)
{
}

void Xeph2D::Runtime::Update()
{
}

void Xeph2D::Runtime::Terminate()
{
}

#else

void Xeph2D::Runtime::Initialize(
	std::function<std::unordered_map<uint32_t, std::string>(void)> namingCallback,
	std::function<void(std::shared_ptr<Component>& ptr, uint32_t compID)> populateCallback)
{
	WindowManager::Initialize();
	AssetManager::Initialize();
	SceneManager::Initialize(namingCallback, populateCallback);
	SceneManager::LoadScene(0);
}

void Xeph2D::Runtime::Update()
{
	while (WindowManager::IsOpen())
	{
		WindowManager::CheckWindowEvents();
		Time::Update();
		InputSystem::Update();

		SceneManager::ActiveScene().Update();
		Debug::Update();

		WindowManager::Begin();
		RenderStack::Draw();
		Debug::DrawToWindow();
		WindowManager::End();
	}
}

void Xeph2D::Runtime::Terminate()
{
}

#endif //_EDITOR
