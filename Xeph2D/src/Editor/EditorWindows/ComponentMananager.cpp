#ifdef IS_EDITOR
#include "Xeph2D/Editor/EditorWindows/ComponentManager.h"

#include "Xeph2D/Editor/Editor.h"
#include "Xeph2D/Systems/Debug.h"
#include "Xeph2D/Utility.h"
#include "Xeph2D/Systems/AppData.h"

#include <filesystem>
#include <fstream>
#include <regex>

#define MANIFEST_PATH "settings/ComponentManifest.yaml"
#define GEN_PATH "gen/ComponentManifest.generated.h"
#define COMPS_DIR "Assets/Components/"

#define TEMPL_H_PATH "templ/comp_h.template"
#define TEMPL_CPP_PATH "templ/comp_cpp.template"

void Xeph2D::Edit::ComponentManager::Initialize()
{
	name = "Script Manager";
	Close();
	LoadFromFile();
}

void Xeph2D::Edit::ComponentManager::OnGUI()
{
	std::vector<uint32_t> ids;
	std::vector<std::string> names;

	for (auto& script : m_userComponents)
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
		Editor::Get().GetComponentCreator()->Open();
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
		if (ImGui::Button("Confirm##RemoveScript"))
		{

			std::filesystem::remove(COMPS_DIR + m_userComponents[ids[m_editSelection]].path + names[m_editSelection] + ".h");
			std::filesystem::remove(COMPS_DIR + m_userComponents[ids[m_editSelection]].path + names[m_editSelection] + ".cpp");
			m_userComponents.erase(ids[m_editSelection]);
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
		ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1.f }, "Assets/Components/");
		ImGui::SameLine();
		ImGui::InputText("##Path", m_pathBuffer, 1024);
		ImGui::NewLine();
		if (ImGui::Button("Save##Script"))
		{
			bool wasChanged = EditComponent(ids[m_editSelection]);
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

std::string Xeph2D::Edit::ComponentManager::GetComponentName(uint32_t typeID) const
{
	if (m_defaultComponents.find(typeID) != m_defaultComponents.end())
		return m_defaultComponents.at(typeID);

	if (m_userComponents.find(typeID) != m_userComponents.end())
		return m_userComponents.at(typeID).name;

	return std::string();
}

std::vector<std::string> Xeph2D::Edit::ComponentManager::GetAllNames()
{
	std::vector<std::string> result;

	for (auto& scriptPair : m_defaultComponents)
		result.push_back(scriptPair.second);
	for (auto& scriptPair : m_userComponents)
		result.push_back(scriptPair.second.name);

	return result;
}

uint32_t Xeph2D::Edit::ComponentManager::GetID(std::string name) const
{
	auto default_iter = std::find_if(m_defaultComponents.begin(), m_defaultComponents.end(), [&](const auto& c) { return c.second == name; });
	if (default_iter != m_defaultComponents.end())
		return default_iter->first;

	auto user_iter = std::find_if(m_userComponents.begin(), m_userComponents.end(), [&](const auto& c) { return c.second.name == name; });
	if (user_iter != m_userComponents.end())
		return user_iter->first;

	return 0;
}

void Xeph2D::Edit::ComponentManager::LoadFromFile()
{
	Markup::Node info = AppData::Load(AppData::DataFile::ComponentManifest);
	for (Markup::Node script : info["default"])
	{
		m_defaultComponents[Utility::FromHex32String(script["id"].As<std::string>())] = script["name"].As<std::string>();
	}
	for (Markup::Node script : info["user"])
	{
		uint32_t id = Utility::FromHex32String(script["id"].As<std::string>());
		m_userComponents[id].name = script["name"].As<std::string>();
		if (script["path"].IsDefined())
			m_userComponents[id].path = script["path"].As<std::string>();
	}
}

void Xeph2D::Edit::ComponentManager::SaveToFile()
{
	Markup::Node info;

	for (const auto& scriptPair : m_defaultComponents)
	{
		Markup::Node scriptInfo;
		scriptInfo["id"] = Utility::ToHex32String(scriptPair.first);
		scriptInfo["name"] = scriptPair.second;
		info["default"].PushBack(scriptInfo);
	}
	for (const auto& scriptPair : m_userComponents)
	{
		Markup::Node scriptInfo;
		scriptInfo["id"] = Utility::ToHex32String(scriptPair.first);
		scriptInfo["name"] = scriptPair.second.name;
		if (!scriptPair.second.path.empty())
			scriptInfo["path"] = scriptPair.second.path;
		info["user"].PushBack(scriptInfo);
	}

	AppData::Save(AppData::DataFile::ComponentManifest, info);
}

void Xeph2D::Edit::ComponentManager::GenerateHeader()
{
	if (!std::filesystem::exists("gen/"))
		std::filesystem::create_directories("gen/");

	std::ofstream file(GEN_PATH);

	file << R"(#pragma once

//ComponentManifest.generated.h is Auto-Generated and managed by the Xeph2D Editor.
//Use the Editor to add or remove scripts

#include <Xeph2D.h>

#include <memory>
#include <cstdint>
)" << '\n';
	for (auto& scriptPair : m_userComponents)
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

	for (auto& scriptPair : m_defaultComponents)
	{
		file << "        case " << Utility::ToHex32String(scriptPair.first) << ": ptr = std::make_shared<" << scriptPair.second << ">(); break;\n";
	}
	for (auto& scriptPair : m_userComponents)
	{
		file << "        case " << Utility::ToHex32String(scriptPair.first) << ": ptr = std::make_shared<" << scriptPair.second.name << ">(); break;\n";
	}
	file << "        }\n    }\n}\n";
}

void Xeph2D::Edit::ComponentManager::CreateNew(const std::string& name, std::string& path)
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
		isUsed = (m_userComponents.find(newID) != m_userComponents.end()
			|| m_defaultComponents.find(newID) != m_defaultComponents.end());
	}

	m_userComponents[newID] = { name, path };

	GenerateFiles(name, path, newID);
	Editor::Save();
	SaveToFile();
	GenerateHeader();
}

void Xeph2D::Edit::ComponentManager::GenerateFiles(const std::string& name, const std::string& path, uint32_t newID)
{
	if (!std::filesystem::exists(COMPS_DIR + path))
		std::filesystem::create_directories(COMPS_DIR + path);

	std::stringstream id;
	id << std::setw(8) << std::setfill('0') << std::hex << newID;

	std::ifstream fileIn(TEMPL_H_PATH);
	std::ofstream fileOut(COMPS_DIR + path + name + ".h");
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
	fileOut.open(COMPS_DIR + path + name + ".cpp");
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

bool Xeph2D::Edit::ComponentManager::EditComponent(uint32_t id)
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

	bool doChangePath = (path != m_userComponents[id].path);
	bool doChangeName = (newName != m_userComponents[id].name);

	if (!doChangeName && !doChangeName)
		return false;

	std::ifstream headerIn(COMPS_DIR + m_userComponents[id].path + m_userComponents[id].name + ".h");
	std::ifstream cppIn(COMPS_DIR + m_userComponents[id].path + m_userComponents[id].name + ".cpp");
	std::stringstream headerTemp;
	std::stringstream cppTemp;
	headerTemp << headerIn.rdbuf();
	cppTemp << cppIn.rdbuf();
	headerIn.close();
	cppIn.close();
	std::filesystem::remove(COMPS_DIR + m_userComponents[id].path + m_userComponents[id].name + ".h");
	std::filesystem::remove(COMPS_DIR + m_userComponents[id].path + m_userComponents[id].name + ".cpp");

	if (!std::filesystem::exists(COMPS_DIR + path))
		std::filesystem::create_directories(COMPS_DIR + path);
	std::ofstream out(COMPS_DIR + path + newName + ".h");
	std::string line;
	std::regex pattern(m_userComponents[id].name);
	while (std::getline(headerTemp, line))
	{
		line = std::regex_replace(line, pattern, newName);
		out << line << std::endl;
	}
	out.close();
	out.open(COMPS_DIR + path + newName + ".cpp");
	while (std::getline(cppTemp, line))
	{
		if (line == "#include \"" + m_userComponents[id].path + m_userComponents[id].name + ".h\"")
		{
			out << "#include \"" + path + newName + ".h\"" << std::endl;
			continue;
		}
		line = std::regex_replace(line, pattern, newName);
		out << line << std::endl;
	}
	out.close();

	m_userComponents[id].path = path;
	m_userComponents[id].name = newName;

	return true;
}

#endif //IS_EDITOR