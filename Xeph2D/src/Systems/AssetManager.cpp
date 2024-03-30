#include "Xeph2D/Systems/AssetManager.h"

#include "Xeph2D/Systems/Debug.h"

#include "../res/no_image_png.h"
#include "../res/JetBrainsMono_ttf.h"

#include "Xeph2D/Systems/AppData.h"

#define ASSET_MANIFEST_FILE "settings/AssetManifest.yaml"
#define TEXTURE_DIR "Assets/Textures/"
#define FONT_DIR "Assets/Fonts/"

void Xeph2D::AssetManager::Initialize()
{
	if (!Get().m_textureManifest.empty())
	{
		Get().m_textureManifest.clear();
		Debug::LogErr("AssetManifest -> Texture manifesst was already Initialized");
	}

	res::no_image_png(Get().m_defaultTextureData, Get().m_defaultTextureSize);
	Get().m_defaultTexture = std::make_unique<sf::Texture>();
	Get().m_defaultTexture->loadFromMemory(Get().m_defaultTextureData.get(), Get().m_defaultTextureSize);

	res::JetBrainsMono_ttf(Get().m_defaultFontData, Get().m_defaultFontSize);
	Get().m_defaultFont = std::make_unique<sf::Font>();
	Get().m_defaultFont->loadFromMemory(Get().m_defaultFontData.get(), Get().m_defaultFontSize);

	Markup::Node assetManifest = AppData::Load(AppData::DataFile::AssetManifest);
	if (assetManifest["textures"].IsDefined())
	{
		for (Markup::Node& texture : assetManifest["textures"])
		{
			std::string key = texture["key"].As<std::string>();
			std::string path = texture["path"].As<std::string>();
			Get().m_textureManifest[key] = path;
		}
	}
	if (assetManifest["fonts"].IsDefined())
	{
		for (Markup::Node& font : assetManifest["fonts"])
		{
			std::string key = font["key"].As<std::string>();
			std::string path = font["path"].As<std::string>();
			Get().m_fontManifest[key] = path;
		}
	}
}

sf::Texture* Xeph2D::AssetManager::GetTexture(const std::string& key)
{
	if (Get().m_loadedTextures.find(key) == Get().m_loadedTextures.end())
	{
		Debug::LogErr("AssetManager -> Could not find loaded texture with key: %s", key.c_str());
		return Get().m_defaultTexture.get();
	}

	return Get().m_loadedTextures.at(key).get();
}

sf::Font* Xeph2D::AssetManager::GetFont(const std::string& key)
{
	AssetManager& am = AssetManager::Get();
	if (Get().m_loadedFonts.find(key) == Get().m_loadedFonts.end())
	{
		Debug::LogErr("AssetManager -> Could not find loaded font with key: %s", key.c_str());
		return Get().m_defaultFont.get();
	}

	return Get().m_loadedFonts.at(key).get();
}

#ifdef IS_EDITOR
void Xeph2D::AssetManager::SaveToFile()
{
	Markup::Node manifestInfo;
	for (auto& texPair : m_textureManifest)
	{
		Markup::Node texInfo;
		texInfo["key"] = texPair.first;
		texInfo["path"] = texPair.second;
		manifestInfo["textures"].PushBack(texInfo);
	}

	for (auto& fontPair : m_fontManifest)
	{
		Markup::Node fontInfo;
		fontInfo["key"] = fontPair.first;
		fontInfo["path"] = fontPair.second;
		manifestInfo["fonts"].PushBack(fontInfo);
	}

	AppData::Save(AppData::DataFile::AssetManifest, manifestInfo);
}
#endif //IS_EDITOR

void Xeph2D::AssetManager::LoadTexture(const std::string& key)
{
	m_loadedTextures[key] = std::make_unique<sf::Texture>();
#ifndef IS_DEBUG
	AppData::GetAssetData(AssetType::Texture, key, m_loadedTexturesData[key]);
	m_loadedTextures[key]->loadFromMemory(m_loadedTexturesData[key]->data(), m_loadedTexturesData[key]->size());
#else // IS_DEBUG
	m_loadedTextures[key]->loadFromFile(TEXTURE_DIR + Get().m_textureManifest[key]);
#endif //!IS_DEBUG
}

void Xeph2D::AssetManager::UnloadTexture(const std::string& key)
{
	m_loadedTextures.erase(key);
#ifndef IS_DEBUG
	m_loadedTexturesData.erase(key);
#endif //!IS_DEBUG
}

void Xeph2D::AssetManager::ClearTextures()
{
	m_loadedTextures.clear();
#ifndef IS_DEBUG
	m_loadedTexturesData.clear();
#endif //!IS_DEBUG
}

void Xeph2D::AssetManager::LoadFont(const std::string& key)
{
	m_loadedFonts[key] = std::make_unique<sf::Font>();
#ifndef IS_DEBUG
	AppData::GetAssetData(AssetType::Font, key, m_loadedFontsData[key]);
	m_loadedFonts[key]->loadFromMemory(m_loadedFontsData[key]->data(), m_loadedFontsData[key]->size());
#else // IS_DEBUG
	m_loadedFonts[key]->loadFromFile(FONT_DIR + Get().m_fontManifest[key]);
#endif //!IS_DEBUG
}

void Xeph2D::AssetManager::UnloadFont(const std::string& key)
{
	m_loadedFonts.erase(key);
#ifndef IS_DEBUG
	m_loadedFontsData.erase(key);
#endif //!IS_DEBUG
}

void Xeph2D::AssetManager::ClearFonts()
{
	m_loadedFonts.clear();
#ifndef IS_DEBUG
	m_loadedFontsData.clear();
#endif //!IS_DEBUG
}
