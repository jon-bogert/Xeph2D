#include "AppData.h"

#include "Debug.h"

#include <xe-markup/Format/BSON.h>
#include <xe-markup/Format/YAML.h>

#include <filesystem>


#define ASSET_MANIFEST_FILE "settings/AssetManifest.yaml"
#define BUILD_INFO_FILE "settings/BuildInfo.yaml"
#define WINDOW_PROPERTIES_FILE "settings/WindowProperties.yaml"

#ifdef _EDITOR
#define COMPONENT_MANIFEST_FILE "settings/ComponentManifest.yaml"
#define EDITOR_FILE "debug/Editor.yaml"
#endif //_EDITOR

#define RELEASE_FILE "core.dat"
#define RELEASE_GRAPHICS_FILE "graphics.cfg"
#define BUILD_DATA_DIRECTORY std::string("../bin/Release-windows-x86_64/data/")
#define BUILD_DATA_LOCAL_DIRECTORY std::string("data/")


#ifdef _DEBUG

Xeph2D::Markup::Node Xeph2D::AppData::Load(const DataFile file)
{
    Markup::YAMLFormatter yaml;
    Markup::Node result;

    switch (file)
    {
    case DataFile::AssetManifest:
        result = yaml.LoadFromFile(ASSET_MANIFEST_FILE);
        break;
    case DataFile::BuildInfo:
        result = yaml.LoadFromFile(BUILD_INFO_FILE);
        break;
    case DataFile::WindowProperties:
        result = yaml.LoadFromFile(WINDOW_PROPERTIES_FILE);
        break;
#ifdef _EDITOR
    case DataFile::ComponentManifest:
        result = yaml.LoadFromFile(COMPONENT_MANIFEST_FILE);
        break;
    case DataFile::Editor:
        result = yaml.LoadFromFile(EDITOR_FILE);
        break;
#endif //_EDITOR
    default:
        Debug::LogErr("AppData::Load (Debug) -> DataFile enum not implemented");
        break;
    }

    return result;
}

#ifdef _EDITOR

void Xeph2D::AppData::Save(const DataFile file, const Markup::Node& node)
{
    Markup::YAMLFormatter yaml;
    switch (file)
    {
    case DataFile::AssetManifest:
        yaml.SaveToFile(node, ASSET_MANIFEST_FILE);
        break;
    case DataFile::BuildInfo:
        yaml.SaveToFile(node, BUILD_INFO_FILE);
        break;
    case DataFile::WindowProperties:
        yaml.SaveToFile(node, WINDOW_PROPERTIES_FILE);
        break;
    case DataFile::ComponentManifest:
        yaml.SaveToFile(node, COMPONENT_MANIFEST_FILE);
        break;
    case DataFile::Editor:
        yaml.SaveToFile(node, EDITOR_FILE);
        break;
    default:
        Debug::LogErr("AppData::Save (Debug) -> DataFile enum not implemented");
        break;
    }
}

void Xeph2D::AppData::BuildForRelease()
{
    Markup::Node root;

    root["asset-manifest"] = Load(DataFile::AssetManifest);
    root["build-info"] = Load(DataFile::BuildInfo);

    Markup::Node window = Load(DataFile::WindowProperties);
    Markup::Node graphics;
    root["window-properties"]["title"] = window["title"];
    root["window-properties"]["ref-resolution"]["width"] = window["ref-resolution"]["width"];
    root["window-properties"]["ref-resolution"]["height"] = window["ref-resolution"]["height"];
    root["window-properties"]["ppu"] = window["ppu"];

    graphics["resolution-width"] = window["resolution"]["width"];
    graphics["resolution-height"] = window["resolution"]["height"];
    graphics["window-style"] = window["style"];
    graphics["lock-framerate"] = window["lock-framerate"];

    if (!std::filesystem::exists(BUILD_DATA_DIRECTORY))
        std::filesystem::create_directories(BUILD_DATA_DIRECTORY);

    Markup::BSONFormatter bson;
    bson.SaveToFile(root, BUILD_DATA_DIRECTORY + RELEASE_FILE);
    bson.SaveToFile(root, BUILD_DATA_LOCAL_DIRECTORY + RELEASE_FILE);

    Markup::YAMLFormatter yaml;
    yaml.SaveToFile(graphics, BUILD_DATA_DIRECTORY + RELEASE_GRAPHICS_FILE);
    yaml.SaveToFile(graphics, BUILD_DATA_LOCAL_DIRECTORY + RELEASE_GRAPHICS_FILE);
}

#endif //_EDITOR

#else //!_DEBUG

Xeph2D::Markup::Node Xeph2D::AppData::Load(const DataFile file)
{
    if (!Get().m_data.IsDefined())
    {
        Debug::LogErr("AppData::Load -> AppData file was not loaded or was cleared");
        return Markup::Node();
    }
    
    Markup::Node result;
    switch (file)
    {
    case DataFile::AssetManifest:
        result = Get().m_data["asset-manifest"];
        break;
    case DataFile::BuildInfo:
        result = Get().m_data["build-info"];
        break;
    case DataFile::WindowProperties:
        result = Get().m_data["window-properties"];
        break;
    default:
        Debug::LogErr("AppData::Loadd (Release) -> DataFile enum not implemented");
        break;

        return result;
    }
}

void Xeph2D::AppData::Initialize()
{
    Markup::BSONFormatter bson;
    Get().m_data = bson.LoadFromFile(BUILD_DATA_LOCAL_DIRECTORY + RELEASE_FILE);

    Markup::YAMLFormatter yaml;
    Markup::Node graphics = yaml.LoadFromFile(BUILD_DATA_LOCAL_DIRECTORY + RELEASE_GRAPHICS_FILE);

    Get().m_data["window-properties"]["resolution"]["width"] = graphics["resolution-width"];
    Get().m_data["window-properties"]["resolution"]["height"] = graphics["resolution-height"];
    Get().m_data["window-properties"]["style"] = graphics["window-style"];
    Get().m_data["window-properties"]["lock-framerate"] = graphics["lock-framerate"];
}

void Xeph2D::AppData::ClearData()
{
    Get().m_data.Clear();
}

#endif //_DEBUG
