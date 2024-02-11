#ifndef __X2D_EDITOR_EDITOR_H__
#define __X2D_EDITOR_EDITOR_H__

#ifdef _EDITOR

#include "Xeph2D/Editor/EditorSceneData.h"
#include "Xeph2D/Editor/EditorWindows/Viewport.h"
#include "Xeph2D/Editor/EditorWindows/Hierarchy.h"
#include "Xeph2D/Editor/EditorWindows/Inspector.h"
#include "Xeph2D/Editor/EditorWindows/ComponentCreator.h"
#include "Xeph2D/Editor/EditorWindows/ComponentManager.h"
#include "Xeph2D/Editor/EditorWindows/AssetManagerWindow.h"
#include "Xeph2D/Editor/EditorWindows/ProjectSettings.h"
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

        static void RebuildProject() { Get().m_rebuildProject = true; }

    private:
        Editor() {}
        static Editor& Get() { static Editor instance; return instance; }

        friend class Runtime;
        static void Initialize();
        static void InputProc();
        static void Update();
        static void OnGUI();
        static void Draw();
        static void Terminate();


        static void AddObject();
        static void RemoveObject(int index);
        static bool ObjectOrderUp(int index);
        static bool ObjectOrderDown(int index);

        static void AddComponent(int index, uint32_t compID);
        static void RemoveComponent(int objIndex, int compIndex);
        static bool ComponentOrderUp(int objIndex, int compIndex);
        static bool ComponentOrderDown(int objIndex, int compIndex);

        void RemoveAllComponents(uint32_t id);
        void DoProjectRebuild();
        void ClearSceneData();

        bool m_showSaveWindow = false;
        std::function<void(void)> m_saveAction;
        bool m_handleSaveAction = false;

        friend class WindowManager;
        Transform* GetViewportTransform() { return &m_viewportTransform; }

        void YAMLSaver(YAML::Node& node, const Field& field);

        std::unique_ptr<sf::RenderWindow> m_window;
        HWND m_handle = nullptr;

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
        ComponentManager* m_componentManagerWindow;
        ComponentCreator* m_componentCreatorWindow;
        AssetManagerWindow* m_assetManagerWindow;
        ProjectSettings* m_projectSettingsWindow;

        std::unique_ptr<TransformGizmo> m_transformGizmo;

        friend class Hierarchy;
        friend class ComponentCreator;
        friend class ComponentManager;
        friend class ProjectSettings;
        Viewport* GetViewportWindow() { return m_viewportWindow; }
        Inspector* GetInspectorWindow() { return m_inspectorWindow; }
        TransformGizmo* GetTransformGizmo() { return m_transformGizmo.get(); }
        ComponentCreator* GetComponentCreator() { return m_componentCreatorWindow; }
        ComponentManager* GetComponentManager() { return m_componentManagerWindow; }


        std::unique_ptr<sf::Font> m_font = nullptr;
        std::unique_ptr<unsigned char[]> m_fontData = nullptr;
        size_t m_fontDataLength = 0;
        void SetUIStyle();

        bool m_rebuildProject = false;
    };
}
#endif //_EDITOR
#endif //!__X2D_EDITOR_EDITOR_H__