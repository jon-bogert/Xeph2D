#ifndef __XEPH2D_SYSTEMS_SCENELOADER_H__
#define __XEPH2D_SYSTEMS_SCENELOADER_H__

#ifdef _EDITOR
#include "Xeph2D/Editor/EditorSceneData.h"
#endif //_EDITOR

#include <Xeph2D/Structs.h>
#include "Serialize.macros.h"

#include <yaml-cpp/yaml.h>

#include <memory>
#include <string>
#include <vector>
#include <functional>

#ifdef _DEBUG
#define EDITOR_FILE "debug/Editor.yaml"
#endif //_DEBUG

namespace Xeph2D
{
#ifdef _EDITOR
    namespace Edit
    {
        struct EditorGameObject;
        class Editor;
        class ProjectSettings;
    }
#endif //_EDITOR
    class Scene;
    class Component;

    using ScenePath = std::string;
    class SceneManager final
    {
    public:
        ~SceneManager() = default;
        SceneManager(const SceneManager& other) = delete;
        SceneManager(const SceneManager&& other) = delete;
        SceneManager operator=(const SceneManager& other) = delete;
        SceneManager operator=(const SceneManager&& other) = delete;

        static Scene& ActiveScene();
        static int ActiveSceneIndex();
        static void LoadScene(const std::string& sceneName);
        static void LoadScene(const int buildIndex);

        //To be accessed by derived Components (cannot be friend)
        static void __Deserialize(SerializableType type, void* ptr, const std::string& field);

    private:
        SceneManager() {}
        static SceneManager& Get() { static SceneManager instance; return instance; }

        friend class Runtime;
        static void Initialize(
            std::function<void(std::shared_ptr<Component>& ptr, uint32_t compID)> populateCallback
        );

        void LoadSceneFile(const std::string& filePath, const int buildIndex, bool isFullPath = false);

        YAML::iterator::value_type* m_componentInfoBuffer = nullptr;

#ifdef _EDITOR
        Edit::EditorComponent* m_editorCompBuffer = nullptr;

        friend class Edit::Editor;
        friend class Edit::ProjectSettings;
        std::shared_ptr<Component>& AddComponentByID(const int gameObjectIndex, const uint32_t typeID);
        void SaveSceneManifest();
        static void OpenSceneWindow();
        static void EmptyScene();
#endif //_EDITOR
#ifdef _DEBUG
        static void LoadLast();
#endif //_DEBUG

        using SceneManifest = std::vector<ScenePath>;
        SceneManifest m_manifest;
        int m_index = -1;
        std::shared_ptr<Scene> m_activeScene;

        std::function<void(std::shared_ptr<Component>& ptr, uint32_t compID)> m_populateCallback;
    };
}

#endif // !__XEPH2D_SYSTEMS_SCENELOADER_H__