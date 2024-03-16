#ifndef __XEPH2D_SYSTEMS_SCENELOADER_H__
#define __XEPH2D_SYSTEMS_SCENELOADER_H__

#ifdef IS_EDITOR
#include "Xeph2D/Editor/EditorSceneData.h"
#endif //IS_EDITOR

#include <Xeph2D/Structs.h>
#include "Serialize.macros.h"
#include "../Systems/AppData.h"

#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Xeph2D
{
#ifdef IS_EDITOR
    namespace Edit
    {
        struct EditorGameObject;
        class Editor;
        class ProjectSettings;
    }
#endif //IS_EDITOR
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

        Markup::Node* m_componentInfoBuffer = nullptr;

#ifdef IS_EDITOR
        Edit::EditorComponent* mIS_EDITORCompBuffer = nullptr;

        friend class Edit::Editor;
        friend class Edit::ProjectSettings;
        std::shared_ptr<Component>& AddComponentByID(const int gameObjectIndex, const uint32_t typeID);
        void SaveSceneManifest();
        static void OpenSceneWindow();
        static void EmptyScene();
#endif //IS_EDITOR
#ifdef IS_DEBUG
        static void LoadLast();
#endif //IS_DEBUG

        using SceneManifest = std::vector<ScenePath>;
        SceneManifest m_manifest;
        int m_index = -1;
        std::shared_ptr<Scene> m_activeScene;

        std::function<void(std::shared_ptr<Component>& ptr, uint32_t compID)> m_populateCallback;
    };
}

#endif // !__XEPH2D_SYSTEMS_SCENELOADER_H__