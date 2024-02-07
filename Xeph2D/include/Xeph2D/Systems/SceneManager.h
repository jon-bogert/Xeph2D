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

namespace Xeph2D
{
#ifdef _EDITOR
    namespace Edit
    {
        struct EditorGameObject;
    }
#endif //_EDITOR
    class Scene;
    class Component;

    using SceneInfo = std::string;
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

        using SceneManifest = std::vector<SceneInfo>;
        SceneManifest m_manifest;
        int m_index = -1;
        std::shared_ptr<Scene> m_activeScene;

        YAML::iterator::value_type* m_componentInfoBuffer = nullptr;
#ifdef _EDITOR
        Edit::EditorComponent* m_editorCompBuffer= nullptr;
#endif //_EDITOR

        std::function<void(std::shared_ptr<Component>& ptr, uint32_t compID)> m_populateCallback;
    };
}

#endif // !__XEPH2D_SYSTEMS_SCENELOADER_H__