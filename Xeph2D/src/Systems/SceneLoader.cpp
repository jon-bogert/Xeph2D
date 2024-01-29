#include "Xeph2D/Systems/SceneLoader.h"

#include "Xeph2D/Component.h"
#include "Xeph2D/Scene.h"
#include "Xeph2D/Systems/Debug.h"
#include "Xeph2D/Utility.h"

#include "CustomSerialTypes.h"

#include <filesystem>

using yaml_val = YAML::iterator::value_type;

Xeph2D::Scene& Xeph2D::SceneLoader::ActiveScene()
{
    return *Get().m_activeScene;
}

int Xeph2D::SceneLoader::ActiveSceneIndex()
{
    return Get().m_index;
}

void Xeph2D::SceneLoader::LoadScene(const std::string& sceneName)
{
    for (size_t i = 0; i < Get().m_manifest.size(); ++i)
    {
        if (Get().m_manifest[i] == sceneName)
        {
            LoadScene(i);
            return;
        }
    }
    Debug::LogErr("Could not find scene with name: %s", sceneName.c_str());
}

void Xeph2D::SceneLoader::LoadScene(const int buildIndex)
{
    if (Get().m_activeScene != nullptr)
    {
        //TODO - Check Asset similarities
        Get().m_activeScene->Shutdown();
    }

    std::string filePath = "Assets/Scenes/" + Get().m_manifest[buildIndex];
    if (!std::filesystem::exists(filePath))
    {
        Debug::LogErr("SceneLoader -> Could not find Scene file at path: %s", filePath.c_str());
        return;
    }

    Get().m_index = buildIndex;
    
    YAML::Node contents;
    contents = YAML::LoadFile(filePath);

    //TODO - Load Scene Data

    for (yaml_val& objInfo : contents["objects"])
    {
        std::shared_ptr<GameObject>& activeObject = Get().m_activeScene->m_gameObjects.emplace_back();
        activeObject->m_instID = Utility::FromHex32String(objInfo["instID"].as<std::string>());
        activeObject->m_name = objInfo["name"].as<std::string>();
        activeObject->m_transform = CustomSerialTypes::TransformFromYAML(objInfo["transform"]);
        activeObject->m_isActive = objInfo["active"].as<bool>();

        if (objInfo["components"].IsDefined())
        {
            for (yaml_val& compInfo : objInfo["components"])
            {
                std::shared_ptr<Component>& activeComponent = activeObject->m_components.emplace_back();
                activeComponent->m_enabled = compInfo["enabled"].as<bool>();
                activeComponent->gameObject = Ref<GameObject>(activeObject);
                Get().m_componentInfoBuffer = &compInfo;
                activeComponent->Serializables();
                Get().m_componentInfoBuffer = nullptr;
            }
        }
    }
}

void Xeph2D::SceneLoader::__Deserialize(SerializableType type, void* ptr, const std::string& field)
{
    if (m_componentInfoBuffer == nullptr)
    {
        Debug::LogErr("SceneLoader -> Component info buffer was null while Loading");
        return;
    }

    switch (type)
    {
    case SerializableType::Int:
        *static_cast<int*>(ptr) = (*Get().m_componentInfoBuffer)[field].as<int>();
        break;
    case SerializableType::Float:
        *static_cast<float*>(ptr) = (*Get().m_componentInfoBuffer)[field].as<float>();
        break;
    case SerializableType::Bool:
        *static_cast<bool*>(ptr) = (*Get().m_componentInfoBuffer)[field].as<bool>();
        break;
    case SerializableType::Char:
        *static_cast<char*>(ptr) = (*Get().m_componentInfoBuffer)[field].as<char>();
        break;
    case SerializableType::String:
        *static_cast<std::string*>(ptr) = (*Get().m_componentInfoBuffer)[field].as<std::string>();
        break;
    case SerializableType::Vector2:
        *static_cast<Vector2*>(ptr) = CustomSerialTypes::Vector2FromYAML((*Get().m_componentInfoBuffer)[field.c_str()]);
        break;
    case SerializableType::Color:
        *static_cast<Color*>(ptr) = CustomSerialTypes::ColorFromYAML((*Get().m_componentInfoBuffer)[field.c_str()]);
        break;
    case SerializableType::Transform:
        *static_cast<Transform*>(ptr) = CustomSerialTypes::TransformFromYAML((*Get().m_componentInfoBuffer)[field.c_str()]);
        break;
    default:
        Debug::LogErr("SceneLoader::__Deserialize -> Unimplemented Type");
        break;
    }
}

void Xeph2D::SceneLoader::Initialize(
    std::function<std::unordered_map<uint32_t, std::string>(void)> namingCallback,
    std::function<void(std::shared_ptr<Component>& ptr, uint32_t compID)> populateCallback)
{
    Get().m_namingCallback = namingCallback;
    Get().m_populateCallback = populateCallback;

    //TODO - Load from file
    Get().m_manifest.push_back("Main.yaml");
}
