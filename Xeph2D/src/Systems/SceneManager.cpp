#include "Xeph2D/Systems/SceneManager.h"

#include "Xeph2D/Component.h"
#include "Xeph2D/Scene.h"
#include "Xeph2D/Systems/AssetManager.h"
#include "Xeph2D/Systems/Debug.h"
#include "Xeph2D/Utility.h"
#ifdef _EDITOR
#include "Xeph2D/Editor/Editor.h"
#endif //_EDITOR

#include "CustomSerialTypes.h"

#include <filesystem>

#define BUILD_INFO_FILE "settings/BuildInfo.yaml"

Xeph2D::Scene& Xeph2D::SceneManager::ActiveScene()
{
    return *Get().m_activeScene;
}

int Xeph2D::SceneManager::ActiveSceneIndex()
{
    return Get().m_index;
}

void Xeph2D::SceneManager::LoadScene(const std::string& sceneName)
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

void Xeph2D::SceneManager::LoadScene(const int buildIndex)
{
    if (Get().m_activeScene != nullptr)
    {
        //TODO - Check Asset similarities
        Get().m_activeScene->Shutdown();
        AssetManager::Get().ClearTextures();
    }
    Get().m_activeScene = std::make_shared<Scene>();

    std::string filePath = "Assets/Scenes/" + Get().m_manifest[buildIndex];
    if (!std::filesystem::exists(filePath))
    {
        Debug::LogErr("SceneLoader -> Could not find Scene file at path: %s", filePath.c_str());
        return;
    }

    Get().m_index = buildIndex;
    
    YAML::Node contents;
    contents = YAML::LoadFile(filePath);

    Get().m_activeScene->m_name = Get().m_manifest[buildIndex];

    //Load Assets
    for (yaml_val& texture : contents["textures"])
    {
        AssetManager::Get().LoadTexture(texture.as<std::string>());
    }

    //Load Objects
    for (yaml_val& objInfo : contents["objects"])
    {
        std::shared_ptr<GameObject>& activeObject = Get().m_activeScene->m_gameObjects.emplace_back(std::make_shared<GameObject>());
        activeObject->m_instID = Utility::FromHex32String(objInfo["instID"].as<std::string>());
        activeObject->m_name = objInfo["name"].as<std::string>();
        activeObject->m_transform = CustomSerialTypes::TransformFromYAML(objInfo["transform"]);
        activeObject->m_isActive = objInfo["active"].as<bool>();

#ifdef _EDITOR
        Edit::Editor& editor = Edit::Editor::Get();
        Edit::EditorGameObject edActiveObject = 
            editor.m_sceneData.gameObjects.emplace_back();

        edActiveObject.instID = activeObject->m_instID;
        edActiveObject.name.ptr = &activeObject->m_name;
        edActiveObject.transform.ptr = &activeObject->m_transform;
        edActiveObject.isActive.ptr = &activeObject->m_isActive;
#endif //_EDITOR

        if (objInfo["components"].IsDefined())
        {
            for (yaml_val& compInfo : objInfo["components"])
            {
                uint32_t typeID = Utility::FromHex32String(compInfo["typeID"].as<std::string>());
                std::shared_ptr<Component>& activeComponent = activeObject->m_components.emplace_back();
                Get().m_populateCallback(activeComponent, typeID);
                activeComponent->m_enabled = compInfo["enabled"].as<bool>();
                activeComponent->gameObject = Ref<GameObject>(activeObject);
                Get().m_componentInfoBuffer = &compInfo;
#ifdef _EDITOR
                Edit::EditorComponent& edActiveComp =
                    edActiveObject.components.emplace_back();
                edActiveComp.typeID = typeID;
                edActiveComp.enabled.ptr = &activeComponent->m_enabled;
                Get().m_editorCompBuffer = &edActiveComp;
                activeComponent->Serializables();
                Get().m_editorCompBuffer = nullptr;
#else
                activeComponent->Serializables();
#endif //_EDITOR
                Get().m_componentInfoBuffer = nullptr;
            }
        }
    }

    Get().m_activeScene->Initialize();
}

void Xeph2D::SceneManager::__Deserialize(SerializableType type, void* ptr, const std::string& field)
{
    if (Get().m_componentInfoBuffer == nullptr)
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

#ifdef _EDITOR
        Edit::Field& edField = Get().m_editorCompBuffer->fields.emplace_back();
        edField.name = field;
        edField.type = type;
        edField.ptr = ptr;
#endif //_EDITOR
    }
}

void Xeph2D::SceneManager::Initialize(
    std::function<std::unordered_map<uint32_t, std::string>(void)> namingCallback,
    std::function<void(std::shared_ptr<Component>& ptr, uint32_t compID)> populateCallback)
{
    Get().m_namingCallback = namingCallback;
    Get().m_populateCallback = populateCallback;

    YAML::Node buildInfo = YAML::LoadFile(BUILD_INFO_FILE);
    for (yaml_val& scene : buildInfo["scenes"])
        Get().m_manifest.push_back(scene.as<std::string>());
}
