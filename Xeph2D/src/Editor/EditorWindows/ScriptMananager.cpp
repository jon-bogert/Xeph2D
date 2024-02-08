#ifdef _EDITOR
#include "Xeph2D/Editor/EditorWindows/ScriptManager.h"

#include "Xeph2D/Editor/Editor.h"
#include "Xeph2D/Systems/Debug.h"
#include "Xeph2D/Utility.h"

#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <fstream>
#include <regex>

#define MANIFEST_PATH "settings/ScriptManifest.yaml"
#define GEN_PATH "gen/ScriptManifest.generated.h"
#define SCRIPTS_DIR "Assets/Scripts/"

#define TEMPL_H_PATH "templ/comp_h.template"
#define TEMPL_CPP_PATH "templ/comp_cpp.template"

void Xeph2D::Edit::ScriptManager::Initialize()
{
	name = "Script Manager";
	Close();
	LoadFromFile();
}

void Xeph2D::Edit::ScriptManager::OnGUI()
{
	std::vector<uint32_t> ids;
	std::vector<std::string> names;

	for (auto& script : m_userScripts)
	{
		ids.push_back(script.first);
		names.push_back(script.second.name);
	}
	if (ImGui::ListBox("##HItems", &m_editSelection, Utility::CStrVect(names).data(), names.size()))
	{
		m_isRemoving = false;
	}

	ImGui::NewLine();
	if (ImGui::Button("Add##Script"))
	{
		Editor::Get().GetScriptCreator()->Open();
	}
	ImGui::SameLine();
	if (ImGui::Button("Remove##Script"))
	{
		if (m_editSelection >= 0)
		{
			m_isRemoving = true;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Edit##Script"))
	{
		if (m_editSelection >= 0)
		{
			m_isEditing = true;
		}
	}

	if (m_isRemoving)
	{
		ImGui::SetNextWindowPos({ (ImGui::GetMainViewport()->Size.x - ImGui::GetWindowWidth()) * 0.5f, (ImGui::GetMainViewport()->Size.y - ImGui::GetWindowHeight()) * 0.5f });
		ImGui::Begin("Confirm Remove##Script", &m_isRemoving,
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoDecoration);

		ImGui::Text(("Are you sure you want to remove " + names[m_editSelection] + "?").c_str());
		ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1.0f }, "Notes: ");
		ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1.0f }, " - Your current scene will also be saved.");
		ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1.0f }, " - Remember to remove the script file references\nfrom your Visual Studio Project.");
		if (ImGui::Button("Confirm##RemoveScript"))
		{

			std::filesystem::remove(SCRIPTS_DIR + m_userScripts[ids[m_editSelection]].path + names[m_editSelection] + ".h");
			std::filesystem::remove(SCRIPTS_DIR + m_userScripts[ids[m_editSelection]].path + names[m_editSelection] + ".cpp");
			m_userScripts.erase(ids[m_editSelection]);
			Editor::Get().RemoveAllComponents(ids[m_editSelection]);
			Editor::Save();
			SaveToFile();
			GenerateHeader();
			m_editSelection = -1;
			m_isRemoving = false;
			Editor::RebuildProject();
			Editor::Close();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			m_isRemoving = false;
		}
		ImGui::End();
	}

	if (m_isEditing)
	{
		ImGui::SetNextWindowPos({ (ImGui::GetMainViewport()->Size.x - ImGui::GetWindowWidth()) * 0.5f, (ImGui::GetMainViewport()->Size.y - ImGui::GetWindowHeight()) * 0.5f });
		ImGui::Begin("Edit##Script", &m_isRemoving,
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoDecoration);

		//if (InputSystem::GetMouseDown(Mouse::Button::Left) && !ImGui::IsWindowHovered())
		//{
		//	m_isEditing = false;
		//	strcpy(m_nameBuffer, "");
		//	strcpy(m_pathBuffer, "");
		//}

		ImGui::Text(("Editing: " + names[m_editSelection]).c_str());
		std::stringstream id;
		id << "Component ID: " << std::setw(8) << std::setfill('0') << std::hex << ids[m_editSelection];
		ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1.0f }, id.str().c_str());
		ImGui::NewLine();
		ImGui::Text("Name:");
		ImGui::InputText("##Name", m_nameBuffer, 256);
		ImGui::NewLine();
		ImGui::Text("Path:");
		ImGui::SameLine();
		ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1.f }, "(optional)");
		ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1.f }, "Assets/Scripts/");
		ImGui::SameLine();
		ImGui::InputText("##Path", m_pathBuffer, 1024);
		ImGui::NewLine();
		if (ImGui::Button("Save##Script"))
		{
			bool wasChanged = EditScript(ids[m_editSelection]);
			if (wasChanged)
			{
				Editor::Save();
				SaveToFile();
				GenerateHeader();
				m_isEditing = false;
				strcpy(m_nameBuffer, "");
				strcpy(m_pathBuffer, "");
				m_editSelection = -1;
				Editor::RebuildProject();
				Editor::Close();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel##Script"))
		{
			m_isEditing = false;
			strcpy(m_nameBuffer, "");
			strcpy(m_pathBuffer, "");
		}
		ImGui::End();
	}
}

std::string Xeph2D::Edit::ScriptManager::GetScriptName(uint32_t typeID) const
{
	if (m_defaultScripts.find(typeID) != m_defaultScripts.end())
		return m_defaultScripts.at(typeID);

	if (m_userScripts.find(typeID) != m_userScripts.end())
		return m_userScripts.at(typeID).name;

	return std::string();
}

std::vector<std::string> Xeph2D::Edit::ScriptManager::GetAllNames()
{
	std::vector<std::string> result;

	for (auto& scriptPair : m_defaultScripts)
		result.push_back(scriptPair.second);
	for (auto& scriptPair : m_userScripts)
		result.push_back(scriptPair.second.name);

	return result;
}

uint32_t Xeph2D::Edit::ScriptManager::GetID(std::string name) const
{
	auto default_iter = std::find_if(m_defaultScripts.begin(), m_defaultScripts.end(), [&](const auto& c) { return c.second == name; });
	if (default_iter != m_defaultScripts.end())
		return default_iter->first;

	auto user_iter = std::find_if(m_userScripts.begin(), m_userScripts.end(), [&](const auto& c) { return c.second.name == name; });
	if (user_iter != m_userScripts.end())
		return user_iter->first;

	return 0;
}

void Xeph2D::Edit::ScriptManager::LoadFromFile()
{
	if (!std::filesystem::exists(MANIFEST_PATH))
	{
		Debug::LogErr("Script Manager -> Could not find Manifest file");
		return;
	}

	YAML::Node info = YAML::LoadFile(MANIFEST_PATH);
	for (yaml_val script : info["default"])
	{
		m_defaultScripts[Utility::FromHex32String(script["id"].as<std::string>())] = script["name"].as<std::string>();
	}
	for (yaml_val script : info["user"])
	{
		uint32_t id = Utility::FromHex32String(script["id"].as<std::string>());
		m_userScripts[id].name = script["name"].as<std::string>();
		if (script["path"].IsDefined())
			m_userScripts[id].path = script["path"].as<std::string>();
	}
}

void Xeph2D::Edit::ScriptManager::SaveToFile()
{
	YAML::Node info;

	for (const auto& scriptPair : m_defaultScripts)
	{
		YAML::Node scriptInfo;
		scriptInfo["id"] = Utility::ToHex32String(scriptPair.first);
		scriptInfo["name"] = scriptPair.second;
		info["default"].push_back(scriptInfo);
	}
	for (const auto& scriptPair : m_userScripts)
	{
		YAML::Node scriptInfo;
		scriptInfo["id"] = Utility::ToHex32String(scriptPair.first);
		scriptInfo["name"] = scriptPair.second.name;
		if (!scriptPair.second.path.empty())
			scriptInfo["path"] = scriptPair.second.path;
		info["user"].push_back(scriptInfo);
	}

	std::ofstream file(MANIFEST_PATH);
	file << info;
	file.close();
}

void Xeph2D::Edit::ScriptManager::GenerateHeader()
{
	if (!std::filesystem::exists("gen/"))
		std::filesystem::create_directories("gen/");

	std::ofstream file(GEN_PATH);

	file << R"(#pragma once

//ScriptManifest.generated.h is Auto-Generated and managed by the Xeph2D Editor.
//Use the Editor to add or remove scripts

#include <Xeph2D.h>

#include <memory>
#include <cstdint>
)" << '\n';
	for (auto& scriptPair : m_userScripts)
	{
		file << "#include \"" << scriptPair.second.path << scriptPair.second.name << ".h\"\n";
	}
	file << R"(
#define __X2D_POPULATE_COMP_PTR &Xeph2D::__PopulateComponentPtr

namespace Xeph2D
{
    void __PopulateComponentPtr(std::shared_ptr<Component>& ptr, uint32_t compID)
    {
        switch (compID)
        {)" << '\n';

	for (auto& scriptPair : m_defaultScripts)
	{
		file << "        case " << Utility::ToHex32String(scriptPair.first) << ": ptr = std::make_shared<" << scriptPair.second << ">(); break;\n";
	}
	for (auto& scriptPair : m_userScripts)
	{
		file << "        case " << Utility::ToHex32String(scriptPair.first) << ": ptr = std::make_shared<" << scriptPair.second.name << ">(); break;\n";
	}
	file << "        }\n    }\n}\n";
}

void Xeph2D::Edit::ScriptManager::CreateNew(const std::string& name, std::string& path)
{
	if (path != "")
	{
		if (path.back() != '/' && path.back() != '\\')
			path.push_back('/');

		for (size_t i = 0; i < path.length(); ++i)
		{
			if (path[i] == '\\')
				path[i] = '/';
		}
	}

	uint32_t newID;
	bool isUsed = true;
	while (isUsed)
	{
		newID = Math::Random::UInt32();
		isUsed = (m_userScripts.find(newID) != m_userScripts.end()
			|| m_defaultScripts.find(newID) != m_defaultScripts.end());
	}

	m_userScripts[newID] = { name, path };

	GenerateFiles(name, path, newID);
	Editor::Save();
	SaveToFile();
	GenerateHeader();
}

void Xeph2D::Edit::ScriptManager::GenerateFiles(const std::string& name, const std::string& path, uint32_t newID)
{
	if (!std::filesystem::exists(SCRIPTS_DIR + path))
		std::filesystem::create_directories(SCRIPTS_DIR + path);

	std::stringstream id;
	id << std::setw(8) << std::setfill('0') << std::hex << newID;

	std::ifstream fileIn(TEMPL_H_PATH);
	std::ofstream fileOut(SCRIPTS_DIR + path + name + ".h");
	std::string line;
	std::regex pattern;
	while (std::getline(fileIn, line))
	{
		pattern = "@NAME@";
		line = std::regex_replace(line, pattern, name);
		pattern = "@ID@";
		line = std::regex_replace(line, pattern, "0x" + id.str());

		fileOut << line << std::endl;
	}

	fileIn.close();
	fileOut.close();
	fileIn.open(TEMPL_CPP_PATH);
	fileOut.open(SCRIPTS_DIR + path + name + ".cpp");
	while (std::getline(fileIn, line))
	{
		pattern = "@NAME@";
		line = std::regex_replace(line, pattern, name);
		pattern = "@ID@";
		line = std::regex_replace(line, pattern, "0x" + id.str());

		fileOut << line << std::endl;
	}

	fileIn.close();
	fileOut.close();
}

bool Xeph2D::Edit::ScriptManager::EditScript(uint32_t id)
{
	std::string path(m_pathBuffer);
	std::string newName(m_nameBuffer);

	if (path != "")
	{
		if (path.back() != '/' && path.back() != '\\')
			path.push_back('/');

		for (size_t i = 0; i < path.length(); ++i)
		{
			if (path[i] == '\\')
				path[i] = '/';
		}
	}

	bool doChangePath = (path != m_userScripts[id].path);
	bool doChangeName = (newName != m_userScripts[id].name);

	if (!doChangeName && !doChangeName)
		return false;

	std::ifstream headerIn(SCRIPTS_DIR + m_userScripts[id].path + m_userScripts[id].name + ".h");
	std::ifstream cppIn(SCRIPTS_DIR + m_userScripts[id].path + m_userScripts[id].name + ".cpp");
	std::stringstream headerTemp;
	std::stringstream cppTemp;
	headerTemp << headerIn.rdbuf();
	cppTemp << cppIn.rdbuf();
	headerIn.close();
	cppIn.close();
	std::filesystem::remove(SCRIPTS_DIR + m_userScripts[id].path + m_userScripts[id].name + ".h");
	std::filesystem::remove(SCRIPTS_DIR + m_userScripts[id].path + m_userScripts[id].name + ".cpp");

	if (!std::filesystem::exists(SCRIPTS_DIR + path))
		std::filesystem::create_directories(SCRIPTS_DIR + path);
	std::ofstream out(SCRIPTS_DIR + path + newName + ".h");
	std::string line;
	std::regex pattern(m_userScripts[id].name);
	while (std::getline(headerTemp, line))
	{
		line = std::regex_replace(line, pattern, newName);
		out << line << std::endl;
	}
	out.close();
	out.open(SCRIPTS_DIR + path + newName + ".cpp");
	while (std::getline(cppTemp, line))
	{
		if (line == "#include \"" + m_userScripts[id].path + m_userScripts[id].name + ".h\"")
		{
			out << "#include \"" + path + newName + ".h\"" << std::endl;
			continue;
		}
		line = std::regex_replace(line, pattern, newName);
		out << line << std::endl;
	}
	out.close();

	m_userScripts[id].path = path;
	m_userScripts[id].name = newName;

	return true;
}

#endif //_EDITOR