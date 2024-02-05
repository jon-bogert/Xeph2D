#ifndef __X2D_EDITOR_EDITOR_H__
#define __X2D_EDITOR_EDITOR_H__

#ifdef _EDITOR

#include "Xeph2D/Editor/EditorSceneData.h"

#include <SFML.hpp>

#include <functional>
#include <string>
#include <memory>
#include <Windows.h>

namespace Xeph2D::Edit
{
    class Editor final
    {
    public:
        ~Editor() = default;
        Editor(const Editor& other) = delete;
        Editor(const Editor&& other) = delete;
        Editor operator=(const Editor& other) = delete;
        Editor operator=(const Editor&& other) = delete;

        static bool IsOpen();
        static void Close();

    private:
        Editor() {}
        static Editor& Get() { static Editor instance; return instance; }

        friend class Runtime;
        static void Initialize(std::function<std::unordered_map<uint32_t, std::string>(void)>& compNameCallback);
        static void Update();
        static void OnGUI();
        static void Draw();
        static void Terminate();

        friend class WindowManager;
        Transform* GetViewportTransform() { return &m_viewportTransform; }

        std::unique_ptr<sf::RenderWindow> m_window;
        HWND m_handle = nullptr;

        std::unordered_map<uint32_t, std::string> m_componentNames;

        Transform m_viewportTransform;

        friend class SceneManager;
        EditorSceneData m_sceneData;
    };
}
#endif //_EDITOR
#endif //!__X2D_EDITOR_EDITOR_H__