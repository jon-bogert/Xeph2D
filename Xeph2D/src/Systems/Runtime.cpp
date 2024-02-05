#include "Xeph2D/Systems/Runtime.h"

#ifdef _EDITOR
#include "Xeph2D/Editor/Editor.h"
#endif //_EDITOR

#include "Xeph2D/Systems/AssetManager.h"
#include "Xeph2D/Systems/SceneManager.h"
#include "Xeph2D/Scene.h"
#include "Xeph2D.h"

using namespace Xeph2D;

#ifdef _EDITOR
using namespace Xeph2D::Edit;

void Xeph2D::Runtime::Initialize(
	std::function<std::unordered_map<uint32_t, std::string>(void)> namingCallback,
	std::function<void(std::shared_ptr<Component>& ptr, uint32_t compID)> populateCallback)
{
	Editor::Initialize(namingCallback);
	AssetManager::Initialize();
	SceneManager::Initialize(namingCallback, populateCallback);
	SceneManager::LoadScene(0);
}

void Xeph2D::Runtime::Update()
{
	while (Editor::IsOpen())
	{
		Editor::Update();
		Editor::OnGUI();
		Editor::Draw();
	}
}

void Xeph2D::Runtime::Terminate()
{
	Editor::Terminate();
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
