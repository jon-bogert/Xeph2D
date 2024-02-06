#ifndef __X2D_EDITOR_EDITOR_H__
#define __X2D_EDITOR_EDITOR_H__

#ifdef _EDITOR

#include "Xeph2D/Editor/EditorSceneData.h"
#include "Xeph2D/Editor/EditorWindows/Viewport.h"
#include "Xeph2D/Editor/EditorWindows/Hierarchy.h"
#include "Xeph2D/Editor/EditorWindows/Inspector.h"
#include "Xeph2D/Editor/TransformGizmo.h"

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

        static bool IsSaved() { return Get().m_isSaved; }
        static void SetIsSaved(const bool isSaved) { Get().m_isSaved = isSaved; };
        static void Save();

    private:
        Editor() {}
        static Editor& Get() { static Editor instance; return instance; }

        friend class Runtime;
        static void Initialize(std::function<std::unordered_map<uint32_t, std::string>(void)>& compNameCallback);
        static void InputProc();
        static void Update();
        static void OnGUI();
        static void Draw();
        static void Terminate();

        bool m_showSaveWindow = false;

        friend class WindowManager;
        Transform* GetViewportTransform() { return &m_viewportTransform; }

        void YAMLSaver(YAML::Node& node, const Field& field);

        std::unique_ptr<sf::RenderWindow> m_window;
        HWND m_handle = nullptr;

        std::unordered_map<uint32_t, std::string> m_componentNames;

        Transform m_viewportTransform;
        sf::Clock m_frameTimer;

        friend class SceneManager;
        friend class Inspector;
        EditorSceneData m_sceneData;

        bool m_isSaved = true;

        //Windows
        using EditorWindows = std::vector<std::unique_ptr<EditorWindow>>;
        EditorWindows m_editorWindows;
        Viewport* m_viewportWindow;
        Hierarchy* m_hierarchyWindow;
        Inspector* m_inspectorWindow;

        std::unique_ptr<TransformGizmo> m_transformGizmo;

        friend class Hierarchy;
        Inspector* GetInspectorWindow() { return m_inspectorWindow; }
        TransformGizmo* GetTransformGizmo() { return m_transformGizmo.get(); }


        std::unique_ptr<sf::Font> m_font = nullptr;
        std::unique_ptr<unsigned char[]> m_fontData = nullptr;
        size_t m_fontDataLength = 0;
        void SetUIStyle();
    };
}
#endif //_EDITOR
#endif //!__X2D_EDITOR_EDITOR_H__