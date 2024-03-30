#ifdef IS_EDITOR
#include "Xeph2D/Editor/EditorWindows/AssetManagerWindow.h"

#include "Xeph2D/Systems/AssetManager.h"
#include "Xeph2D/Systems/SceneManager.h"
#include "Xeph2D/Scene.h"
#include "Xeph2D/Utility.h"
#include "Xeph2D/Editor/Editor.h"
#include "Xeph2D/Editor/FileBrowser.h"

#include "Xeph2D/Components/SpriteRenderer.h"

#include <filesystem>
#include <algorithm>

void Xeph2D::Edit::AssetManagerWindow::Initialize()
{
	name = "Asset Manager";
	Close();

	m_textures.clear();

	AssetManager& assetManager = AssetManager::Get();
	for (auto& texPair : assetManager.m_textureManifest)
	{
		m_textures.emplace_back(texPair.first, texPair.second, false);
	}
	for (auto& texPair : assetManager.m_loadedTextures)
	{
		auto iter = std::find_if(m_textures.begin(), m_textures.end(), [&](const Entry& e) { return e.key == texPair.first; });
		iter->isInScene = true;
	}
}

void Xeph2D::Edit::AssetManagerWindow::OnGUI()
{
	AssetManager& assetManager = AssetManager::Get();
#pragma region(TEXTURES_UI)
	if (ImGui::CollapsingHeader("Textures"))
	{
		//for (Entry& entry : m_textures)
		//{
		//	if (ImGui::Checkbox(("##in" + entry.key).c_str(), &entry.isInScene))
		//	{
		//		//Add or remove from assetmanager loaded stuff
		//	}
		//}

		if (ImGui::BeginTable("Texture Table", 4, ImGuiTableFlags_Resizable))
		{
			ImGui::TableSetupColumn("In Scene");
			ImGui::TableSetupColumn("Key");
			ImGui::TableSetupColumn("Path");
			ImGui::TableSetupColumn("");
			ImGui::TableHeadersRow();

			int count = 0;
			for (Entry& entry : m_textures)
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				if (m_isEditing && count == m_editIndex)
				{
					ImGui::TableSetColumnIndex(1);
					ImGui::InputText("##keyedit", m_keyBuffer, 255);
					ImGui::TableSetColumnIndex(2);
					ImGui::Text(entry.path.c_str());
					ImGui::TableSetColumnIndex(3);
					if (ImGui::Button("Apply"))
					{
						m_isEditing = false;
						AssetManager& assetManager = AssetManager::Get();
						assetManager.m_textureManifest[m_keyBuffer] = assetManager.m_textureManifest[entry.key];
						assetManager.m_textureManifest.erase(entry.key);

						if (assetManager.m_loadedTextures.find(entry.key) != assetManager.m_loadedTextures.end())
						{
							assetManager.m_loadedTextures[m_keyBuffer] = std::make_unique<sf::Texture>(*assetManager.m_loadedTextures[entry.key]);
							assetManager.m_loadedTextures.erase(entry.key);
						}

						std::vector<Ref<SpriteRenderer>> renderers = SceneManager::ActiveScene().FindObjectsOfType<SpriteRenderer>();
						for (Ref<SpriteRenderer>& renderer : renderers)
						{
							if (renderer->GetTextureKey() == entry.key)
								renderer->SetTexture(m_keyBuffer);
						}

						entry.key = m_keyBuffer;

						Editor::SetIsSaved(false);
						assetManager.SetIsSaved(false);
					}
					++count;
					continue;
				}

				if (ImGui::Checkbox(("##in" + entry.key).c_str(), &entry.isInScene))
				{
					Editor::SetIsSaved(false);
					if (entry.isInScene)
						AssetManager::Get().LoadTexture(entry.key);
					else
						AssetManager::Get().UnloadTexture(entry.key);

					std::vector<Ref<SpriteRenderer>> renderers = SceneManager::ActiveScene().FindObjectsOfType<SpriteRenderer>();
					for (Ref<SpriteRenderer>& renderer : renderers)
					{
						renderer->SetTexture(renderer->GetTextureKey());
					}
				}
				ImGui::TableSetColumnIndex(1);
				ImGui::Text(entry.key.c_str());
				ImGui::TableSetColumnIndex(2);
				ImGui::Text(entry.path.c_str());
				ImGui::TableSetColumnIndex(3);
				if (ImGui::Button(("...##ed" + entry.key).c_str()))
				{
					m_editIndex = count;
					m_editType = Type::Texture;
					m_showOptions = true;
					m_optionsPos = (Vector2)ImGui::GetCursorPos() + ImGui::GetWindowPos();
					m_isEditing = false;
				}
				++count;
			}

			ImGui::EndTable();
		}
		if (m_showOptions)
		{
			ImGui::SetNextWindowPos(m_optionsPos);
			ImGui::Begin("Options##Assets", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
			if (InputSystem::GetMouseDown(Mouse::Button::Left) && !ImGui::IsWindowHovered())
				m_showOptions = false;

			if (ImGui::MenuItem("Edit"))
			{
				m_isEditing = true;
				Entry& entry = m_textures[m_editIndex];
				strcpy(m_keyBuffer, entry.key.c_str());
			}
			if (ImGui::MenuItem("Remove"))
			{
				Entry& entry = m_textures[m_editIndex];
				AssetManager::Get().UnloadTexture(entry.key);
				AssetManager::Get().m_textureManifest.erase(entry.key);
				m_textures.erase(m_textures.begin() + m_editIndex);
				Editor::SetIsSaved(false);
				AssetManager::Get().SetIsSaved(false);
				m_showOptions = false;
			}

			ImGui::End();
		}
		if (ImGui::Button("Add Texture"))
		{
			AddTexture();
		}
	}
#pragma endregion
	/// == FONTS UI ==
#pragma region(FONTS_UI)
	if (ImGui::CollapsingHeader("Fonts"))
	{
		if (ImGui::BeginTable("Font Table", 4, ImGuiTableFlags_Resizable))
		{
			ImGui::TableSetupColumn("In Scene");
			ImGui::TableSetupColumn("Key");
			ImGui::TableSetupColumn("Path");
			ImGui::TableSetupColumn("");
			ImGui::TableHeadersRow();

			int count = 0;
			for (Entry& entry : m_fonts)
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				if (m_isEditing && count == m_editIndex)
				{
					ImGui::TableSetColumnIndex(1);
					ImGui::InputText("##fontkeyedit", m_keyBuffer, 255);
					ImGui::TableSetColumnIndex(2);
					ImGui::Text(entry.path.c_str());
					ImGui::TableSetColumnIndex(3);
					if (ImGui::Button("Apply"))
					{
						m_isEditing = false;
						AssetManager& assetManager = AssetManager::Get();
						assetManager.m_fontManifest[m_keyBuffer] = assetManager.m_fontManifest[entry.key];
						assetManager.m_fontManifest.erase(entry.key);

						if (assetManager.m_loadedFonts.find(entry.key) != assetManager.m_loadedFonts.end())
						{
							assetManager.m_loadedFonts[m_keyBuffer] = std::make_unique<sf::Font>(*assetManager.m_loadedFonts[entry.key]);
							assetManager.m_loadedFonts.erase(entry.key);
						}

						std::vector<Ref<TextRenderer>> renderers = SceneManager::ActiveScene().FindObjectsOfType<TextRenderer>();
						for (Ref<TextRenderer>& renderer : renderers)
						{
							if (renderer->GetFontKey() == entry.key)
								renderer->SetFont(m_keyBuffer);
						}

						entry.key = m_keyBuffer;

						Editor::SetIsSaved(false);
						assetManager.SetIsSaved(false);
					}
					++count;
					continue;
				}

				if (ImGui::Checkbox(("##fontin" + entry.key).c_str(), &entry.isInScene))
				{
					Editor::SetIsSaved(false);
					if (entry.isInScene)
						AssetManager::Get().LoadFont(entry.key);
					else
						AssetManager::Get().UnloadFont(entry.key);

					std::vector<Ref<TextRenderer>> renderers = SceneManager::ActiveScene().FindObjectsOfType<TextRenderer>();
					for (Ref<TextRenderer>& renderer : renderers)
					{
						renderer->SetFont(renderer->GetFontKey());
					}
				}
				ImGui::TableSetColumnIndex(1);
				ImGui::Text(entry.key.c_str());
				ImGui::TableSetColumnIndex(2);
				ImGui::Text(entry.path.c_str());
				ImGui::TableSetColumnIndex(3);
				if (ImGui::Button(("...##fonted" + entry.key).c_str()))
				{
					m_editIndex = count;
					m_editType = Type::Font;
					m_showOptions = true;
					m_optionsPos = (Vector2)ImGui::GetCursorPos() + ImGui::GetWindowPos();
					m_isEditing = false;
				}
				++count;
			}

			ImGui::EndTable();
		}
		if (m_showOptions)
		{
			ImGui::SetNextWindowPos(m_optionsPos);
			ImGui::Begin("Options##fontAssets", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
			if (InputSystem::GetMouseDown(Mouse::Button::Left) && !ImGui::IsWindowHovered())
				m_showOptions = false;

			if (ImGui::MenuItem("Edit##FontEdit"))
			{
				m_isEditing = true;
				Entry& entry = m_fonts[m_editIndex];
				strcpy(m_keyBuffer, entry.key.c_str());
			}
			if (ImGui::MenuItem("Remove##FontRemove"))
			{
				Entry& entry = m_fonts[m_editIndex];
				AssetManager::Get().UnloadTexture(entry.key);
				AssetManager::Get().m_fontManifest.erase(entry.key);
				m_fonts.erase(m_textures.begin() + m_editIndex);
				Editor::SetIsSaved(false);
				AssetManager::Get().SetIsSaved(false);
				m_showOptions = false;
			}

			ImGui::End();
		}
		if (ImGui::Button("Add Font"))
		{
			AddFont();
		}
#pragma endregion
	}
}

void Xeph2D::Edit::AssetManagerWindow::AddTexture()
{
	FileBrowser browser;
	std::filesystem::path startPath = "Assets\\Textures\\";
	startPath = std::filesystem::absolute(startPath);
	browser.PushFileType(L"*.bmp;*.png*;.tga;*.jpg;*.gif;*.psd;*.hdr;*.pic;*.pnm", L"Image files");
	browser.SetStartPath(startPath);
	std::filesystem::path path = browser.GetFile();

	//Check path is within the start path
	//if (std::mismatch(startPath.begin(), startPath.end(), path.begin()).first != startPath.end())
	if (path.empty())
		return;
	if (!FileBrowser::IsRelativeTo(path, startPath))
	{
		Debug::LogErr("Asset selected wasn't located in \"Assets\\Textures\"");
		return;
	}
	//change to relative
	path = std::filesystem::relative(path, startPath);

	std::string key = path.stem().u8string();
	AssetManager::Get().m_textureManifest[key] = path.u8string();

	m_textures.emplace_back(key, path.u8string(), false);
	Editor::SetIsSaved(false);
	AssetManager::Get().SetIsSaved(false);
}

void Xeph2D::Edit::AssetManagerWindow::AddFont()
{
	FileBrowser browser;
	std::filesystem::path startPath = "Assets\\Fonts\\";
	startPath = std::filesystem::absolute(startPath);
	browser.PushFileType(L"*.otf;*.ttf", L"Font files");
	browser.SetStartPath(startPath);
	std::filesystem::path path = browser.GetFile();

	//Check path is within the start path
	//if (std::mismatch(startPath.begin(), startPath.end(), path.begin()).first != startPath.end())
	if (path.empty())
		return;
	if (!FileBrowser::IsRelativeTo(path, startPath))
	{
		Debug::LogErr("Asset selected wasn't located in \"Assets\\Fonts\"");
		return;
	}
	//change to relative
	path = std::filesystem::relative(path, startPath);

	std::string key = path.stem().u8string();
	AssetManager::Get().m_fontManifest[key] = path.u8string();

	m_fonts.emplace_back(key, path.u8string(), false);
	Editor::SetIsSaved(false);
	AssetManager::Get().SetIsSaved(false);
}

#endif //IS_EDITOR