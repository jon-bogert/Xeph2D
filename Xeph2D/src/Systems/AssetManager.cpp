#include "Xeph2D/Systems/AssetManager.h"

#include "Xeph2D/Systems/Debug.h"

#include "../res/no_image_png.h"

#include <yaml-cpp/yaml.h>

#define ASSET_MANIFEST_FILE "settings/AssetManifest.yaml"
#define TEXTURE_DIR "Assets/Textures/"

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

	YAML::Node assetManifest = YAML::LoadFile(ASSET_MANIFEST_FILE);
	if (assetManifest["textures"].IsDefined())
	{
		for (yaml_val& texture : assetManifest["textures"])
		{
			std::string key = texture["key"].as<std::string>();
			std::string path = texture["path"].as<std::string>();
			Get().m_textureManifest[key] = path;
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

#ifdef _EDITOR
void Xeph2D::AssetManager::SaveToFile()
{
	YAML::Node manifestInfo;
	for (auto& texPair : m_textureManifest)
	{
		YAML::Node texInfo;
		texInfo["key"] = texPair.first;
		texInfo["path"] = texPair.second;
		manifestInfo["textures"].push_back(texInfo);
	}

	std::ofstream file(ASSET_MANIFEST_FILE);
	file << manifestInfo;
	file.close();
}
#endif //_EDITOR

void Xeph2D::AssetManager::LoadTexture(const std::string& key)
{
	m_loadedTextures[key] = std::make_unique<sf::Texture>();
	m_loadedTextures[key]->loadFromFile(TEXTURE_DIR + Get().m_textureManifest[key]);
}

void Xeph2D::AssetManager::UnloadTexture(const std::string& key)
{
	m_loadedTextures.erase(key);
}

void Xeph2D::AssetManager::ClearTextures()
{
	m_loadedTextures.clear();
}
