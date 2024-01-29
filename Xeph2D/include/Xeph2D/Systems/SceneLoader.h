#ifndef __XEPH2D_SYSTEMS_SCENELOADER_H__
#define __XEPH2D_SYSTEMS_SCENELOADER_H__

#include <Xeph2D/Structs.h>

#include <yaml-cpp/yaml.h>

#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Xeph2D
{
    class Scene;
    class Component;

    using SceneInfo = std::string;
    class SceneLoader final
    {
    public:
        ~SceneLoader() = default;
        SceneLoader(const SceneLoader& other) = delete;
        SceneLoader(const SceneLoader&& other) = delete;
        SceneLoader operator=(const SceneLoader& other) = delete;
        SceneLoader operator=(const SceneLoader&& other) = delete;

        static Scene& ActiveScene();
        static int ActiveSceneIndex();
        static void LoadScene(const std::string& sceneName);
        static void LoadScene(const int buildIndex);

        //To be accessed by derived Components (cannot be friend)
        void __Deserialize(SerializableType type, void* ptr, const std::string& field);

    private:
        SceneLoader() {}
        static SceneLoader& Get() { static SceneLoader instance; return instance; }

        friend class Runtime;
        static void Initialize(
            std::function<std::unordered_map<uint32_t, std::string>(void)> namingCallback,
            std::function<void(std::shared_ptr<Component>& ptr, uint32_t compID)> populateCallback
        );

        using SceneManifest = std::vector<SceneInfo>;
        SceneManifest m_manifest;
        int m_index = -1;
        std::shared_ptr<Scene> m_activeScene;

        YAML::iterator::value_type* m_componentInfoBuffer = nullptr;

        std::function<std::unordered_map<uint32_t, std::string>(void)> m_namingCallback;
        std::function<void(std::shared_ptr<Component>& ptr, uint32_t compID)> m_populateCallback;
    };
}

#endif // !__XEPH2D_SYSTEMS_SCENELOADER_H__