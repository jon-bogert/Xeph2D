#ifdef IS_EDITOR
#include "Xeph2D/Editor/EditorWindows/ProjectSettings.h"

#include "Xeph2D/Utility.h"
#include "Xeph2D/Systems/WindowManager.h"
#include "Xeph2D/Editor/Editor.h"
#include "Xeph2D/Editor/FileBrowser.h"

#include <fstream>
#include <filesystem>

#define SETTINGS_DIR "settings/"

void Xeph2D::Edit::ProjectSettings::Initialize()
{
	name = "Project Settings";

	m_buildData = AppData::Load(AppData::DataFile::BuildInfo);
	m_displayData = AppData::Load(AppData::DataFile::WindowProperties);
	Close();
}

void Xeph2D::Edit::ProjectSettings::OnGUI()
{
	ImGui::Columns(2, "PageColumns", true);
	if (ImGui::ListBox("##PageSelect", &m_indexSelection, Utility::CStrVect(m_pageNames).data(), m_pageNames.size()))
	{

	}
	ImGui::NextColumn();
	
	switch (static_cast<Page>(m_indexSelection))
	{
	case Page::Application:
		ApplicationPage();
		break;
	case Page::Build:
		BuildPage();
		break;
	case Page::Display:
		DisplayPage();
		break;
	default:
		break;

	}
	ImGui::Columns(1);
	if (ImGui::Button("Save"))
	{
		SaveFiles();
		WindowManager::Initialize();
		Editor::Get().GetViewportWindow()->UpdateSize();
		SceneManager::Get().SaveSceneManifest();
	}
}

void Xeph2D::Edit::ProjectSettings::ApplicationPage()
{
	char buffer[256];
	strcpy(buffer, m_displayData["title"].As<std::string>().c_str());
	if (ImGui::InputText("Title", buffer, 255))
	{
		m_displayData["title"] = std::string(buffer);
	}
}

void Xeph2D::Edit::ProjectSettings::DisplayPage()
{
	int resBuffer[2];
	resBuffer[0] = m_displayData["ref-resolution"]["width"].As<int>();
	resBuffer[1] = m_displayData["ref-resolution"]["height"].As<int>();
	if (ImGui::DragInt2("Reference Resolution", resBuffer))
	{
		if (resBuffer[0] < 1)
			resBuffer[0] = 1;
		if (resBuffer[1] < 1)
			resBuffer[1] = 1;
		m_displayData["ref-resolution"]["width"] = resBuffer[0];
		m_displayData["ref-resolution"]["height"] = resBuffer[1];
	}
	int ppuBuffer = m_displayData["ppu"].As<int>();
	if (ImGui::DragInt("Pixels per Unit", &ppuBuffer))
	{
		if (ppuBuffer < 1)
			ppuBuffer = 1;
		m_displayData["ppu"] = ppuBuffer;
	}
	bool lockFPS = m_displayData["lock-framerate"].As<int>() != 0;
	if (ImGui::Checkbox("Lock Framerate", &lockFPS))
	{
		if (lockFPS)
			m_displayData["lock-framerate"] = 60;
		else
			m_displayData["lock-framerate"] = 0;
	}

	if (lockFPS)
	{
		int fpsBuffer = m_displayData["lock-framerate"].As<int>();
		if (ImGui::DragInt("Framerate", &fpsBuffer))
		{
			if (fpsBuffer < 1)
				fpsBuffer = 1;

			m_displayData["lock-framerate"] = fpsBuffer;
		}
	}

	ImGui::NewLine();
	ImGui::Text("Release");
	bool useNativeResolution = m_displayData["resolution"]["height"].As<int>() == 0;
	if (ImGui::Checkbox("Use Native Resolution", &useNativeResolution))
	{
		if (useNativeResolution)
		{
			m_displayData["resolution"]["width"] = 0;
			m_displayData["resolution"]["height"] = 0;
		}
		else
		{
			m_displayData["resolution"]["width"] = m_displayData["ref-resolution"]["width"].As<int>();
			m_displayData["resolution"]["height"] = m_displayData["ref-resolution"]["height"].As<int>();
		}
	}
	if (!useNativeResolution)
	{
		resBuffer[0] = m_displayData["resolution"]["width"].As<int>();
		resBuffer[1] = m_displayData["resolution"]["height"].As<int>();
		if (ImGui::DragInt2("Render Resolution", resBuffer))
		{
			if (resBuffer[0] < 1)
				resBuffer[0] = 1;
			if (resBuffer[1] < 1)
				resBuffer[1] = 1;
			m_displayData["resolution"]["width"] = resBuffer[0];
			m_displayData["resolution"]["height"] = resBuffer[1];
		}
	}
	char windowOptions[] = "Windowed\0Borderless\0Fullscreen\0Windowed With Resize\0Window No Close\0";
	int currentStyleIndex = m_displayData["style"].As<int>();
	if (ImGui::Combo("Window Style", &currentStyleIndex, windowOptions))
	{
		m_displayData["style"] = currentStyleIndex;
	}

	ImGui::NewLine();
	ImGui::Text("Debug");
	resBuffer[0] = m_displayData["debug-resolution"]["width"].As<int>();
	resBuffer[1] = m_displayData["debug-resolution"]["height"].As<int>();
	if (ImGui::DragInt2("Debug Resolution", resBuffer))
	{
		if (resBuffer[0] < 1)
			resBuffer[0] = 1;
		if (resBuffer[1] < 1)
			resBuffer[1] = 1;
		m_displayData["debug-resolution"]["width"] = resBuffer[0];
		m_displayData["debug-resolution"]["height"] = resBuffer[1];
	}
}

void Xeph2D::Edit::ProjectSettings::BuildPage()
{
	SceneManager& sceneManager = SceneManager::Get();
	std::vector<std::string> scenes;
	uint32_t count = 0;
	for (ScenePath& path : sceneManager.m_manifest)
	{
		scenes.push_back(std::to_string(count++) + " - " + path);
	}
	ImGui::Text("Build Scene Order");
	if (ImGui::ListBox("##SceneOrder", &m_buildIndexSelection, Utility::CStrVect(scenes).data(), scenes.size()))
	{
		
	}
	if (ImGui::Button("+##addScene"))
	{
		AddBuildScene();
	}
	ImGui::SameLine();
	if (ImGui::Button("-##remScene"))
	{
		RemoveBuildScene();
	}
	ImGui::SameLine();
	if (ImGui::Button("^##upScene"))
	{
		BuildSceneUp();
	}
	ImGui::SameLine();
	if (ImGui::Button("v##downScene"))
	{
		BuildSceneDown();
	}
}

void Xeph2D::Edit::ProjectSettings::AddBuildScene()
{
	FileBrowser browser;
	browser.PushFileType(L"*.scene", L"Xeph2D Scene");
	browser.PushFileType(L"*.yaml", L"YAML File");

	std::filesystem::path startPath = std::filesystem::absolute("Assets\\Scenes\\");
	browser.SetStartPath(startPath);

	std::filesystem::path filePath = browser.GetFile();
	if (filePath.empty())
		return;
	if (!FileBrowser::IsRelativeTo(filePath, startPath))
	{
		Debug::LogErr("ProjectSettings::AddBuildScene -> Selected file is not in \"Assets\\Scenes\" folder");
		return;
	}

	filePath = std::filesystem::relative(filePath, startPath);

	SceneManager::Get().m_manifest.push_back(filePath.u8string());
}

void Xeph2D::Edit::ProjectSettings::RemoveBuildScene()
{
	if (m_buildIndexSelection < 0)
		return;

	SceneManager::SceneManifest& manifest = SceneManager::Get().m_manifest;
	manifest.erase(manifest.begin() + m_buildIndexSelection);
}

void Xeph2D::Edit::ProjectSettings::BuildSceneUp()
{
	if (m_buildIndexSelection <= 0)
		return;

	SceneManager::SceneManifest& manifest = SceneManager::Get().m_manifest;
	std::swap(manifest[m_buildIndexSelection], manifest[m_buildIndexSelection - 1]);
	--m_buildIndexSelection;
}

void Xeph2D::Edit::ProjectSettings::BuildSceneDown()
{
	SceneManager::SceneManifest& manifest = SceneManager::Get().m_manifest;
	if (m_buildIndexSelection >= manifest.size() - 1)
		return;

	std::swap(manifest[m_buildIndexSelection], manifest[m_buildIndexSelection + 1]);
	++m_buildIndexSelection;
}

void Xeph2D::Edit::ProjectSettings::SaveFiles()
{
	AppData::Save(AppData::DataFile::WindowProperties, m_displayData);
	AppData::Save(AppData::DataFile::BuildInfo, m_buildData);
}
#endif //IS_EDITOR