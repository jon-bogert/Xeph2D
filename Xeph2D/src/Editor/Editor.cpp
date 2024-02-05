#ifdef _EDITOR
#include "Xeph2D/Editor/Editor.h"

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
	Get().m_window->close();
}

void Xeph2D::Edit::Editor::Save()
{
	Debug::Log("Unimplemented");
	SetIsSaved(true);
}

void Xeph2D::Edit::Editor::Initialize(
	std::function<std::unordered_map<uint32_t, std::string>(void)>& compNameCallback)
{
	Get().m_componentNames = compNameCallback();

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
	//Get().m_scriptManager =
	//	(ScriptManager*)Get().m_editorWindows.emplace_back(std::make_unique<ScriptManager>()).get();
	//Get().m_scriptCreator =
	//	(ScriptCreator*)Get().m_editorWindows.emplace_back(std::make_unique<ScriptCreator>()).get();

	for (auto& window : Get().m_editorWindows)
		window->Initialize();
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

	//Get().m_transformGizmo.UpdateMouse(Get().m_viewportWindow->GetMousePos());
	ImGui::SFML::Update(*Get().m_window, Get().m_frameTimer.restart());
}

void Xeph2D::Edit::Editor::OnGUI()
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File##MainMenu"))
	{
		if (ImGui::MenuItem("Save", "Ctrl+S"))
		{
			Debug::Log("Unimplemented");
			//Serializer::SaveToFile(SceneManager::GetCurrentName());
			//Get().m_hasSaved = true;
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
			Debug::Log("Unimplemented");
			//Get().m_scriptCreator->Open();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("View##MainMenu"))
	{
		if (ImGui::BeginMenu("Windows##MainMenu"))
		{
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
				Debug::Log("Unimplemented");
				//Get().m_scriptManager->Open();
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
	
		ImGui::Begin(window->GetName(), &window->isOpen, window->flags);
		window->UpdateValues();
		window->OnGUI();
		ImGui::End();
	}
	//if (Get().m_showSaveWindow)
	//{
	//	ImGui::Begin("Save Your Progress?", &Get().m_showSaveWindow,
	//		ImGuiWindowFlags_AlwaysAutoResize |
	//		ImGuiWindowFlags_NoResize |
	//		ImGuiWindowFlags_NoDocking |
	//		ImGuiWindowFlags_NoCollapse |
	//		ImGuiWindowFlags_NoMove);
	//	ImGui::SetWindowPos({ (ImGui::GetMainViewport()->Size.x - ImGui::GetWindowWidth()) * 0.5f, (ImGui::GetMainViewport()->Size.y - ImGui::GetWindowHeight()) * 0.5f });
	//	ImGui::Text("Would you like to save the Active Scene?");
	//	ImGui::NewLine();
	//	if (ImGui::Button("Save"))
	//	{
	//		Get().m_hasSaved = true;
	//		Serializer::SaveToFile(SceneManager::GetCurrentName());
	//		Close();
	//		Get().m_showSaveWindow = false;
	//	}
	//	ImGui::SameLine();
	//	if (ImGui::Button("Don't Save"))
	//	{
	//		Get().m_hasSaved = true;
	//		Close();
	//		Get().m_showSaveWindow = false;
	//	}
	//	ImGui::SameLine();
	//	if (ImGui::Button("Cancel"))
	//	{
	//		Get().m_showSaveWindow = false;
	//	}
	//	ImGui::End();
	//}
}

void Xeph2D::Edit::Editor::Draw()
{
	WindowManager::Begin();
	RenderStack::Draw();
	WindowManager::End();

	Get().m_window->clear({ 5, 5, 5, 255 });
	ImGui::SFML::Render(*Get().m_window);
	Get().m_window->display();
}

void Xeph2D::Edit::Editor::Terminate()
{
	ImGui::SFML::Shutdown();
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

#endif //_EDITOR
