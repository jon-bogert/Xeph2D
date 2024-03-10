#include "Xeph2D/Systems/Runtime.h"

#ifdef _EDITOR
#include "Xeph2D/Editor/Editor.h"
#endif //_EDITOR

#include "Xeph2D/Systems/AppData.h"
#include "Xeph2D/Systems/AssetManager.h"
#include "Xeph2D/Systems/SceneManager.h"
#include "Xeph2D/Systems/TimerService.h"
#include "Xeph2D/Scene.h"
#include "Xeph2D.h"

using namespace Xeph2D;

#ifdef _EDITOR
using namespace Xeph2D::Edit;

void Xeph2D::Runtime::Initialize(
	std::function<void(std::shared_ptr<Component>& ptr, uint32_t compID)> populateCallback)
{
	WindowManager::Initialize();
	AssetManager::Initialize();
	SceneManager::Initialize(populateCallback);
	SceneManager::LoadLast();
	Editor::Initialize();
}

void Xeph2D::Runtime::Update()
{
	while (Editor::IsOpen())
	{
		Time::Update();
		TimerService::Update();
		InputSystem::Update();
		Editor::Update();
		SceneManager::ActiveScene().Update();
		Debug::Update();
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
	std::function<void(std::shared_ptr<Component>& ptr, uint32_t compID)> populateCallback)
{
#ifdef NDEBUG
	AppData::Initialize();
#endif // NDEBUG
	WindowManager::Initialize();
	AssetManager::Initialize();
	SceneManager::Initialize(populateCallback);
#ifdef _DEBUG
	SceneManager::LoadLast();
#else
	SceneManager::LoadScene(0);
#endif //_DEBUG
#ifdef NDEBUG
	AppData::ClearData();
#endif // NDEBUG
}

void Xeph2D::Runtime::Update()
{
	while (WindowManager::IsOpen())
	{
		WindowManager::CheckWindowEvents();
		Time::Update();
		TimerService::Update();
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

void Xeph2D::Runtime::ParseArgs(int argc, wchar_t* argv[])
{
	for (int i = 0; i < argc; ++i)
	{
		Get().m_args.push_back(argv[i]);
	}
}

void Xeph2D::Runtime::ParseArgs(wchar_t* pCmdLine)
{
	std::wistringstream stream(pCmdLine);
	std::wstring arg;

	while (stream >> arg)
	{
		Get().m_args.push_back(arg);
	}
}

const std::vector<std::wstring>& Xeph2D::Runtime::Args()
{
	return Get().m_args;
}

std::vector<std::string> Xeph2D::Runtime::ArgsShort()
{
	std::vector<std::string> args{};
	for (std::wstring& arg : Get().m_args)
	{
		args.push_back(Utility::ToShortString(arg));
	}

	return args;
}
