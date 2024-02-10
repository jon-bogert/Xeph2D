#ifdef _EDITOR
#include "Xeph2D/Editor/EditorWindows/ProjectSettings.h"

#include "Xeph2D/Utility.h"
#include "Xeph2D/Systems/WindowManager.h"
#include "Xeph2D/Editor/Editor.h"

#include <fstream>

#define SETTINGS_DIR "settings/"

void Xeph2D::Edit::ProjectSettings::Initialize()
{
	name = "Project Settings";

	m_buildData = YAML::LoadFile(SETTINGS_DIR + m_buildFile);
	m_displayData = YAML::LoadFile(SETTINGS_DIR + m_displayFile);
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
	}
}

void Xeph2D::Edit::ProjectSettings::ApplicationPage()
{
	char buffer[256];
	strcpy(buffer, m_displayData["title"].as<std::string>().c_str());
	if (ImGui::InputText("Title", buffer, 255))
	{
		m_displayData["title"] = buffer;
	}
}

void Xeph2D::Edit::ProjectSettings::DisplayPage()
{
	int resBuffer[2];
	resBuffer[0] = m_displayData["ref-resolution"]["width"].as<int>();
	resBuffer[1] = m_displayData["ref-resolution"]["height"].as<int>();
	if (ImGui::DragInt2("Reference Resolution", resBuffer))
	{
		if (resBuffer[0] < 1)
			resBuffer[0] = 1;
		if (resBuffer[1] < 1)
			resBuffer[1] = 1;
		m_displayData["ref-resolution"]["width"] = resBuffer[0];
		m_displayData["ref-resolution"]["height"] = resBuffer[1];
	}
	int ppuBuffer = m_displayData["ppu"].as<int>();
	if (ImGui::DragInt("Pixels per Unit", &ppuBuffer))
	{
		if (ppuBuffer < 1)
			ppuBuffer = 1;
		m_displayData["ppu"] = ppuBuffer;
	}
	bool lockFPS = m_displayData["lock-framerate"].as<int>() != 0;
	if (ImGui::Checkbox("Lock Framerate", &lockFPS))
	{
		if (lockFPS)
			m_displayData["lock-framerate"] = 60;
		else
			m_displayData["lock-framerate"] = 0;
	}

	if (lockFPS)
	{
		int fpsBuffer = m_displayData["lock-framerate"].as<int>();
		if (ImGui::DragInt("Framerate", &fpsBuffer))
		{
			if (fpsBuffer < 1)
				fpsBuffer = 1;

			m_displayData["lock-framerate"] = fpsBuffer;
		}
	}

	ImGui::NewLine();
	ImGui::Text("Release");
	bool useNativeResolution = m_displayData["resolution"]["height"].as<int>() == 0;
	if (ImGui::Checkbox("Use Native Resolution", &useNativeResolution))
	{
		if (useNativeResolution)
		{
			m_displayData["resolution"]["width"] = 0;
			m_displayData["resolution"]["height"] = 0;
		}
		else
		{
			m_displayData["resolution"]["width"] = m_displayData["ref-resolution"]["width"].as<int>();
			m_displayData["resolution"]["height"] = m_displayData["ref-resolution"]["height"].as<int>();
		}
	}
	if (!useNativeResolution)
	{
		resBuffer[0] = m_displayData["resolution"]["width"].as<int>();
		resBuffer[1] = m_displayData["resolution"]["height"].as<int>();
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
	int currentStyleIndex = m_displayData["style"].as<int>();
	if (ImGui::Combo("Window Style", &currentStyleIndex, windowOptions))
	{
		m_displayData["style"] = currentStyleIndex;
	}

	ImGui::NewLine();
	ImGui::Text("Debug");
	resBuffer[0] = m_displayData["debug-resolution"]["width"].as<int>();
	resBuffer[1] = m_displayData["debug-resolution"]["height"].as<int>();
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

}

void Xeph2D::Edit::ProjectSettings::SaveFiles()
{
	std::ofstream file;
	file.open(SETTINGS_DIR + m_displayFile);
	file << m_displayData;
	file.close();
	file.open(SETTINGS_DIR + m_buildFile);
	file << m_buildData;
}
#endif //_EDITOR