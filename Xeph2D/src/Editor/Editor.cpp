#ifdef _EDITOR
#include "Xeph2D/Editor/Editor.h"

#include "Xeph2D/Systems/AssetManager.h"
#include "Xeph2D/Systems/SceneManager.h"
#include "Xeph2D/Scene.h"

#include "../Systems/CustomSerialTypes.h"

#include "../res/BasierSquare_Medium_otf.h"
#include "../res/JetBrainsMono_ttf.h"

#include <imgui-SFML.h>
#include <imgui.h>

#define EDITOR_WINDOW_TITLE "Xeph2D Editor"

bool Xeph2D::Edit::Editor::IsOpen()
{
	return Get().m_window->isOpen();
}

void Xeph2D::Edit::Editor::Close()
{
	if (!Get().m_isSaved)
	{
		Get().m_showSaveWindow = true;
		return;
	}
	Get().m_window->close();
}

void Xeph2D::Edit::Editor::Save()
{
	AssetManager& am = AssetManager::Get();
	if (!AssetManager::Get().m_isSaved)
	{
		AssetManager::Get().m_isSaved = true;
		AssetManager::Get().SaveToFile();
	}

	YAML::Node sceneData;
	//TEXTURES
	for (auto& tex : AssetManager::Get().m_loadedTextures)
	{
		sceneData["textures"].push_back(tex.first);
	}

	//OBJECTS
	for (EditorGameObject& currObject : Get().m_sceneData.gameObjects)
	{
		YAML::Node obj;
		obj["instID"] = Utility::ToHex32String(currObject.instID);
		obj["name"] = currObject.name.As<std::string>();
		CustomSerialTypes::TransformToYAML(obj["transform"], currObject.transform.As<Transform>());
		obj["active"] = currObject.isActive.As<bool>();
		for (EditorComponent currComp : currObject.components)
		{
			YAML::Node comp;
			comp["typeID"] = Utility::ToHex32String(currComp.typeID);
			comp["enabled"] = currComp.enabled.As<bool>();
			for (Field& field : currComp.fields)
			{
				Get().YAMLSaver(comp, field);
			}
			obj["components"].push_back(comp);
		}
		sceneData["objects"].push_back(obj);
	}

	std::string path = "Assets/Scenes/" + SceneManager::ActiveScene().GetName();
	std::ofstream file(path);

	//TODO - if file doesn't exist, open dialogue save there 
	file << sceneData;
	file.close();

	SetIsSaved(true);
}

void Xeph2D::Edit::Editor::Initialize()
{
	Get().m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1920, 1080), EDITOR_WINDOW_TITLE);
	Get().m_handle = FindWindowA(NULL, EDITOR_WINDOW_TITLE);

	ImGui::SFML::Init(*Get().m_window);

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	Get().SetUIStyle();
	
	Get().m_viewportWindow =
		(Viewport*)Get().m_editorWindows.emplace_back(std::make_unique<Viewport>()).get();
	Get().m_inspectorWindow =
		(Inspector*)Get().m_editorWindows.emplace_back(std::make_unique<Inspector>()).get();
	Get().m_hierarchyWindow =
		(Hierarchy*)Get().m_editorWindows.emplace_back(std::make_unique<Hierarchy>()).get();
	Get().m_scriptManagerWindow =
		(ScriptManager*)Get().m_editorWindows.emplace_back(std::make_unique<ScriptManager>()).get();
	Get().m_scriptCreatorWindow =
		(ScriptCreator*)Get().m_editorWindows.emplace_back(std::make_unique<ScriptCreator>()).get();
	Get().m_assetManagerWindow =
		(AssetManagerWindow*)Get().m_editorWindows.emplace_back(std::make_unique<AssetManagerWindow>()).get();

	Get().m_transformGizmo = std::make_unique<TransformGizmo>();

	for (auto& window : Get().m_editorWindows)
		window->Initialize();
}

void Xeph2D::Edit::Editor::InputProc()
{
	if (Get().m_viewportWindow->IsHovered())
	{
		if (InputSystem::GetMouseHold(Mouse::Button::Right) ||
			(InputSystem::GetKeyHold(Key::LAlt) && InputSystem::GetMouseHold(Mouse::Button::Left)))
		{
			Vector2 delta{};
			InputSystem::GetMouseDelta(&delta.x);
			Get().m_viewportTransform.position.x -= WindowManager::PixelToUnit(delta).x;
			Get().m_viewportTransform.position.y += WindowManager::PixelToUnit(delta).y;
		}
	}
	if (Get().m_viewportWindow->IsFocused())
	{
		if (InputSystem::GetKeyDown(Key::W))
		{
			Get().m_transformGizmo->SetMode(TransformGizmo::Mode::Position);
		}
		if (InputSystem::GetKeyDown(Key::E))
		{
			Get().m_transformGizmo->SetMode(TransformGizmo::Mode::Rotation);
		}
		if (InputSystem::GetKeyDown(Key::R))
		{
			Get().m_transformGizmo->SetMode(TransformGizmo::Mode::Scale);
		}
	}
	if (InputSystem::GetKeyHold(Key::Ctrl))
	{
		if (InputSystem::GetKeyHold(Key::Shift))
		{
			if (InputSystem::GetKeyDown(Key::N))
			{
				Get().m_scriptCreatorWindow->Open();
			}
		}
		else //============
		{
			if (InputSystem::GetKeyDown(Key::S))
			{
				Save();
			}
			if (InputSystem::GetKeyDown(Key::Q))
			{
				Close();
			}
		}
	}
}

void Xeph2D::Edit::Editor::Update()
{
	sf::Event winEvent{};
	while (Get().m_window->pollEvent(winEvent))
	{
		ImGui::SFML::ProcessEvent(*Get().m_window, winEvent);

		if (winEvent.type == sf::Event::Closed)
		{
			Close();
		}
	}

	Editor::InputProc();
	Get().m_transformGizmo->UpdateMouse(Get().m_viewportWindow->GetMousePos());
	ImGui::SFML::Update(*Get().m_window, Get().m_frameTimer.restart());
}

void Xeph2D::Edit::Editor::OnGUI()
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File##MainMenu"))
	{
		if (ImGui::MenuItem("Save", "Ctrl+S"))
		{
			Save();
		}
		if (ImGui::MenuItem("Close", "Ctrl+Q"))
		{
			Editor::Close();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Edit##MainMenu"))
	{
		if (ImGui::MenuItem("Create New Script", "Ctrl+Shift+N"))
		{
			Get().m_scriptCreatorWindow->Open();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("View##MainMenu"))
	{
		if (ImGui::BeginMenu("Windows##MainMenu"))
		{
			if (ImGui::MenuItem("Asset Manager"))
			{
				Get().m_assetManagerWindow->Open();
			}
			if (ImGui::MenuItem("Hierarchy"))
			{
				Get().m_hierarchyWindow->Open();
			}
			if (ImGui::MenuItem("Inspector"))
			{
				Debug::Log("Unimplemented");
				Get().m_inspectorWindow->Open();
			}
			if (ImGui::MenuItem("Script Manager"))
			{
				Get().m_scriptManagerWindow->Open();
			}
			if (ImGui::MenuItem("Viewport"))
			{
				Get().m_viewportWindow->Open();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
	ImGui::DockSpaceOverViewport();
	for (auto& window : Get().m_editorWindows)
	{
		if (!window->isOpen)
			continue;
	
		ImGui::Begin(window->GetWindowName(), &window->isOpen, window->flags);
		window->UpdateValues();
		window->OnGUI();
		ImGui::End();
	}
	if (Get().m_showSaveWindow)
	{
		ImGui::Begin("Save Your Progress?", &Get().m_showSaveWindow,
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoMove);
		ImGui::SetWindowPos({ (ImGui::GetMainViewport()->Size.x - ImGui::GetWindowWidth()) * 0.5f, (ImGui::GetMainViewport()->Size.y - ImGui::GetWindowHeight()) * 0.5f });
		ImGui::Text("Would you like to save the Active Scene?");
		ImGui::NewLine();
		if (ImGui::Button("Save"))
		{
			Get().m_isSaved = true;
			Save();
			Close();
			Get().m_showSaveWindow = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Don't Save"))
		{
			Get().m_isSaved = true;
			Close();
			Get().m_showSaveWindow = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			Get().m_showSaveWindow = false;
		}
		ImGui::End();
	}
}

void Xeph2D::Edit::Editor::Draw()
{
	Get().m_viewportWindow->DebugUI();
	Get().m_transformGizmo->Draw();

	WindowManager::Begin();
	RenderStack::Draw();
	Debug::DrawToWindow();
	WindowManager::End();

	Get().m_window->clear({ 5, 5, 5, 255 });
	ImGui::SFML::Render(*Get().m_window);
	Get().m_window->display();
}

void Xeph2D::Edit::Editor::Terminate()
{
	ImGui::SFML::Shutdown();
	if (Get().m_rebuildProject)
		Get().DoProjectRebuild();
}

void Xeph2D::Edit::Editor::AddObject()
{
	SetIsSaved(false);
	GameObject* obj = SceneManager::ActiveScene().CreateObject().m_ptr.lock().get();
	EditorGameObject& edObj = Get().m_sceneData.gameObjects.emplace_back();
	edObj.name.name = "Name";
	edObj.name.type = SerializableType::String;
	edObj.name.ptr = &obj->m_name;
	edObj.isActive.name = "Active";
	edObj.isActive.type = SerializableType::Bool;
	edObj.isActive.ptr = &obj->m_isActive;
	edObj.transform.name = "Transform";
	edObj.transform.type = SerializableType::Transform;
	edObj.transform.ptr = &obj->m_transform;
	edObj.instID = obj->InstID();
}

void Xeph2D::Edit::Editor::RemoveObject(int index)
{
	SetIsSaved(false);
	std::shared_ptr<GameObject>& obj = SceneManager::ActiveScene().m_gameObjects[index];
	SceneManager::ActiveScene().DestroyObject(Ref<GameObject>(obj));
	Get().m_sceneData.gameObjects.erase(Get().m_sceneData.gameObjects.begin() + index);
}

bool Xeph2D::Edit::Editor::ObjectOrderUp(int index)
{
	if (index <= 0)
		return false;

	SetIsSaved(false);
	Scene& s = SceneManager::ActiveScene();
	std::swap(s.m_gameObjects[index], s.m_gameObjects[index - 1]);
	std::swap(Get().m_sceneData.gameObjects[index], Get().m_sceneData.gameObjects[index - 1]);
	return true;
}

bool Xeph2D::Edit::Editor::ObjectOrderDown(int index)
{
	Scene& s = SceneManager::ActiveScene();
	if (index >= s.m_gameObjects.size() - 1)
		return false;

	SetIsSaved(false);
	std::swap(s.m_gameObjects[index], s.m_gameObjects.begin()[index + 1]);
	std::swap(Get().m_sceneData.gameObjects[index], Get().m_sceneData.gameObjects[index + 1]);
	return true;
}

void Xeph2D::Edit::Editor::AddComponent(int index, uint32_t compID)
{
	SetIsSaved(false);
	std::shared_ptr<Component>& comp = SceneManager::Get().AddComponentByID(index, compID);
	comp->gameObject = SceneManager::ActiveScene().m_gameObjects[index];
	
	EditorComponent& edComp = Get().m_sceneData.gameObjects[index].components.emplace_back();
	edComp.typeID = compID;
	edComp.enabled.name = ("Enabled");
	edComp.enabled.type = SerializableType::Bool;
	edComp.enabled.ptr = &comp->m_enabled;

	SceneManager::Get().m_editorCompBuffer = &edComp;
	comp->Serializables();
	SceneManager::Get().m_editorCompBuffer = nullptr;
}

void Xeph2D::Edit::Editor::RemoveComponent(int objIndex, int compIndex)
{
	SetIsSaved(false);
	std::shared_ptr<GameObject>& obj = SceneManager::ActiveScene().m_gameObjects[objIndex];
	obj->DestroyComponent(obj->m_components[compIndex]);
	EditorGameObject& edObj = Get().m_sceneData.gameObjects[objIndex];
	edObj.components.erase(edObj.components.begin() + compIndex);
}

bool Xeph2D::Edit::Editor::ComponentOrderUp(int objIndex, int compIndex)
{
	if (compIndex <= 0)
		return false;

	SetIsSaved(false);
	Scene& s = SceneManager::ActiveScene();
	std::swap(s.m_gameObjects[objIndex]->m_components[compIndex], s.m_gameObjects[objIndex]->m_components[compIndex - 1]);
	std::swap(Get().m_sceneData.gameObjects[objIndex].components[compIndex], Get().m_sceneData.gameObjects[objIndex].components[compIndex - 1]);
	return true;
}

bool Xeph2D::Edit::Editor::ComponentOrderDown(int objIndex, int compIndex)
{
	Scene& s = SceneManager::ActiveScene();
	if (compIndex >= s.m_gameObjects[objIndex]->m_components.size() - 1)
		return false;

	SetIsSaved(false);
	std::swap(s.m_gameObjects[objIndex]->m_components[compIndex], s.m_gameObjects[objIndex]->m_components[compIndex + 1]);
	std::swap(Get().m_sceneData.gameObjects[objIndex].components[compIndex], Get().m_sceneData.gameObjects[objIndex].components[compIndex + 1]);
	return true;
}

void Xeph2D::Edit::Editor::RemoveAllComponents(uint32_t typeID)
{
	Scene& scene = SceneManager::ActiveScene();
	Editor& e = Get();
	for (EditorGameObject& edObj : Get().m_sceneData.gameObjects)
	{
		for (auto iter = edObj.components.begin(); iter != edObj.components.end();)
		{
			if (iter->typeID == typeID)
				iter = edObj.components.erase(iter);
			else
				++iter;
		}
	}
	for (auto& sp_obj : scene.m_gameObjects)
	{
		for (auto iter = sp_obj->m_components.begin(); iter != sp_obj->m_components.end();)
		{
			if (iter->get()->TypeID() == typeID)
			{
				iter->get()->OnEditorShutdown();
				iter = sp_obj->m_components.erase(iter);
			}
			else
				++iter;
		}
	}
}

void Xeph2D::Edit::Editor::YAMLSaver(YAML::Node& node, const Field& field)
{
	switch (field.type)
	{
	case SerializableType::Int:
		node[field.name] = field.As<int>();
		break;
	case SerializableType::Float:
		node[field.name] = field.As<float>();
		break;
	case SerializableType::Bool:
		node[field.name] = field.As<bool>();
		break;
	case SerializableType::Char:
		node[field.name] = field.As<char>();
		break;
	case SerializableType::String:
		node[field.name] = field.As<std::string>();
		break;
	case SerializableType::Vector2:
		CustomSerialTypes::Vector2ToYAML(node[field.name], field.As<Vector2>());
		break;
	case SerializableType::Color:
		CustomSerialTypes::ColorToYAML(node[field.name], field.As<Color>());
		break;
	case SerializableType::Transform:
		CustomSerialTypes::TransformToYAML(node[field.name], field.As<Transform>());
		break;
	default:
		Debug::LogErr("Editor::YAMLSaver -> Unimplemented Type");
	}
}

void Xeph2D::Edit::Editor::SetUIStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowRounding = 4.0f;
	style.ScrollbarSize = 10.0f;

	style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_Border];
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.7f, 0.7f, 0.7f, 0.70f);
	style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	style.GrabRounding = style.FrameRounding = 2.3f;

	//Font
	ImGuiIO& io = ImGui::GetIO();
	ImFontAtlas* fontAtlas = io.Fonts;
	res::BasierSquare_Medium_otf(Get().m_fontData, Get().m_fontDataLength);
	ImFontConfig fontConfig;
	fontConfig.FontDataOwnedByAtlas = false;
	ImWchar defaultRange[] = { 0x0020, 0x00FF, 0x0100, 0x017F, 0 };
	ImFont* font = fontAtlas->AddFontFromMemoryTTF((void*)Get().m_fontData.get(), Get().m_fontDataLength, 16, &fontConfig, defaultRange);
	fontAtlas->Build();
	io.FontDefault = font;

	ImGui::SFML::UpdateFontTexture();
}

void Xeph2D::Edit::Editor::DoProjectRebuild()
{
	int bufsize = GetWindowTextLength(m_handle) + 1;
	char* title = new char[bufsize];
	GetWindowTextA(m_handle, title, bufsize);

	system((std::string("start ..\\reload-sln.exe \"") + title + "\"").c_str());

	delete[] title;
}

#endif //_EDITOR
