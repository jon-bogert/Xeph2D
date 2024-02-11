#include "Xeph2D/Systems/SceneManager.h"

#include "Xeph2D/Component.h"
#include "Xeph2D/Scene.h"
#include "Xeph2D/Systems/AssetManager.h"
#include "Xeph2D/Systems/Debug.h"
#include "Xeph2D/Utility.h"
#ifdef _EDITOR
#include "Xeph2D/Editor/Editor.h"
#include "Xeph2D/Editor/FileBrowser.h"
#endif //_EDITOR

#include "CustomSerialTypes.h"

#include <filesystem>

#define BUILD_INFO_FILE "settings/BuildInfo.yaml"
#define SCENE_DIR "Assets/Scenes/"

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
	std::string filePath = Get().m_manifest[buildIndex];
	Get().LoadSceneFile(filePath, buildIndex);
}

void Xeph2D::SceneManager::__Deserialize(SerializableType type, void* ptr, const std::string& field)
{
	if (Get().m_componentInfoBuffer != nullptr) // will be null if adding a new component in editor
	{
		if ((*Get().m_componentInfoBuffer)[field].IsDefined()) // field could not be defined if added on this compile
		{
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
		else
		{
			Debug::LogColor(Color::Green);
			Debug::Log("Adding new field %s on next save", field.c_str());
		}
	}

#ifdef _EDITOR
	Edit::Field& edField = Get().m_editorCompBuffer->fields.emplace_back();
	edField.name = field;
	edField.type = type;
	edField.ptr = ptr;
#endif //_EDITOR
}

void Xeph2D::SceneManager::Initialize(
	std::function<void(std::shared_ptr<Component>& ptr, uint32_t compID)> populateCallback)
{
	Get().m_populateCallback = populateCallback;

	YAML::Node buildInfo = YAML::LoadFile(BUILD_INFO_FILE);
	if (!buildInfo["scenes"].IsDefined())
		return;

	for (yaml_val& scene : buildInfo["scenes"])
		Get().m_manifest.push_back(scene.as<std::string>());
}

void Xeph2D::SceneManager::LoadSceneFile(const std::string& filePath, const int buildIndex, bool isFullPath)
{
	if (Get().m_activeScene != nullptr)
	{
		//TODO - Check Asset similarities
		Get().m_activeScene->Shutdown();
		AssetManager::Get().ClearTextures();
#ifdef _EDITOR
		Edit::Editor::Get().ClearSceneData();
#endif //_EDITOR
	}
	Get().m_activeScene = std::make_shared<Scene>();

	std::string fullFilePath = (isFullPath) ? filePath : SCENE_DIR + filePath;
	
	if (!std::filesystem::exists(fullFilePath))
	{
		Debug::LogErr("SceneLoader -> Could not find Scene file at path: %s", (fullFilePath).c_str());
		return;
	}

	Get().m_index = buildIndex;

	YAML::Node contents;
	contents = YAML::LoadFile(fullFilePath);

	Get().m_activeScene->m_name = (isFullPath) ?
		"Untitled" :
		filePath;

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
		Edit::EditorGameObject& edActiveObject =
			editor.m_sceneData.gameObjects.emplace_back();

		edActiveObject.instID = activeObject->m_instID;
		edActiveObject.name.ptr = &activeObject->m_name;
		edActiveObject.name.name = "Name";
		edActiveObject.name.type = SerializableType::String;
		edActiveObject.transform.ptr = &activeObject->m_transform;
		edActiveObject.transform.name = "Transform";
		edActiveObject.transform.type = SerializableType::Transform;
		edActiveObject.isActive.ptr = &activeObject->m_isActive;
		edActiveObject.isActive.name = "Active";
		edActiveObject.isActive.type = SerializableType::Bool;
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
				edActiveComp.enabled.name = "Enabled";
				edActiveComp.enabled.type = SerializableType::Bool;
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
	if (!isFullPath)
		Get().m_activeScene->m_hasFile = true;
	Get().m_activeScene->Initialize();
}

#ifdef _EDITOR
std::shared_ptr<Xeph2D::Component>& Xeph2D::SceneManager::AddComponentByID(int gameObjectIndex, uint32_t typeID)
{
	GameObject* obj = ActiveScene().m_gameObjects[gameObjectIndex].get();
	std::shared_ptr<Component>& comp = obj->m_components.emplace_back();
	m_populateCallback(comp, typeID);
	return comp;
}
void Xeph2D::SceneManager::SaveSceneManifest()
{
	YAML::Node manifestInfo;

	for (const ScenePath& path : Get().m_manifest)
	{
		YAML::Node sceneInfo;
		sceneInfo = path;
		manifestInfo["scenes"].push_back(sceneInfo);
	}

	std::ofstream file(BUILD_INFO_FILE);
	file << manifestInfo;
	file.close();
}
void Xeph2D::SceneManager::OpenSceneWindow()
{
	if (!Edit::Editor::Get().m_isSaved)
	{
		Edit::Editor::Get().m_saveAction = SceneManager::OpenSceneWindow;
		Edit::Editor::Get().m_showSaveWindow = true;
		return;
	}

	Edit::FileBrowser browser;
	browser.PushFileType(L"*.scene", L"Xeph2D Scene");
	browser.PushFileType(L"*.yaml", L"YAML File");

	std::filesystem::path startPath = std::filesystem::absolute("Assets\\Scenes\\");
	browser.SetStartPath(startPath);

	std::filesystem::path filePath = browser.GetFile();
	if (filePath.empty())
		return;
	if (!Edit::FileBrowser::IsRelativeTo(filePath, startPath))
	{
		Debug::LogErr("ProjectSettings::AddBuildScene -> Selected file is not in \"Assets\\Scenes\" folder");
		return;
	}

	filePath = std::filesystem::relative(filePath, startPath);
	int index = -1;
	for (int i = 0; i < Get().m_manifest.size(); ++i)
	{
		if (Get().m_manifest[i] == filePath.u8string())
		{
			index = i;
			break;
		}
	}
	Get().LoadSceneFile(filePath.u8string(), index);
	Edit::Editor::Get().m_assetManagerWindow->Initialize();
}
void Xeph2D::SceneManager::EmptyScene()
{
	if (!Edit::Editor::Get().m_isSaved)
	{
		Edit::Editor::Get().m_saveAction = SceneManager::EmptyScene;
		Edit::Editor::Get().m_showSaveWindow = true;
		return;
	}
	Get().LoadSceneFile("templ/scene.template", -1, true);
	Edit::Editor::SetIsSaved(false);
	Edit::Editor::Get().m_assetManagerWindow->Initialize();
}
#endif //_EDITOR

#ifdef _DEBUG
void Xeph2D::SceneManager::LoadLast()
{
	YAML::Node editData = YAML::LoadFile(EDITOR_FILE);

	std::string last = editData["last"].as<std::string>();
	if (!std::filesystem::exists(SCENE_DIR + last))
	{
#ifdef _EDITOR
		EmptyScene();
#endif //_EDITOR
		return;
	}

	int index = -1;
	for (int i = 0; i < Get().m_manifest.size(); ++i)
	{
		if (Get().m_manifest[i] == last)
		{
			index = i;
			break;
		}
	}
	Get().LoadSceneFile(last, index);
}
#endif //_DEBUG
