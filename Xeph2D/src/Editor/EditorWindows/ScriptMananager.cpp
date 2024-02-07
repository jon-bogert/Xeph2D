#ifdef _EDITOR
#include "Xeph2D/Editor/EditorWindows/ScriptManager.h"

#include "Xeph2D/Systems/Debug.h"
#include "Xeph2D/Utility.h"

#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <fstream>

#define MANIFEST_PATH "settings/ScriptManifest.yaml"
#define GEN_PATH "gen/ScriptManifest.generated.h"

void Xeph2D::Edit::ScriptManager::Initialize()
{
	name = "Script Manager";
	LoadFromFile();
}

void Xeph2D::Edit::ScriptManager::OnGUI()
{
	if (ImGui::Button("Regenerate Header"))
	{
		GenerateHeader();
	}
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
		m_userScripts[Utility::FromHex32String(script["id"].as<std::string>())] = script["name"].as<std::string>();
	}
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
#include <unordered_map>
#include <cstdint>
)" << '\n';
	for (auto& scriptPair : m_userScripts)
	{
		file << "#include \"" << scriptPair.second << ".h\"\n";
	}
	file << R"(
#define __X2D_REGISTER_COMP_NAMES &Xeph2D::__RegisterComponentNames
#define __X2D_POPULATE_COMP_PTR &Xeph2D::__PopulateComponentPtr

namespace Xeph2D
{
    std::unordered_map<uint32_t, std::string> __RegisterComponentNames()
    {
        return{)" << '\n';

	for (auto& scriptPair : m_defaultScripts)
	{
		file << "        {" << Utility::ToHex32String(scriptPair.first) << ", \"" << scriptPair.second << "\"},\n";
	}
	for (auto& scriptPair : m_userScripts)
	{
		file << "        {" << Utility::ToHex32String(scriptPair.first) << ", \"" << scriptPair.second << "\"},\n";
	}
	file.seekp(-3, std::ios_base::cur);
	file << "};\n";

	file << R"(    };

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
		file << "        case " << Utility::ToHex32String(scriptPair.first) << ": ptr = std::make_shared<" << scriptPair.second << ">(); break;\n";
	}
	file << "        }\n    }\n}\n";
}

#endif //_EDITOR