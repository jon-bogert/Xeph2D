#include "Xeph2D/Systems/AssetManager.h"

#include "Xeph2D/Systems/Debug.h"
#include <yaml-cpp/yaml.h>

#define ASSET_MANIFEST_FILE "settings/AssetManifest.yaml"

void Xeph2D::AssetManager::Initialize()
{
	if (!Get().m_textureManifest.empty())
	{
		Get().m_textureManifest.clear();
		Debug::LogErr("AssetManifest -> Texture manifesst was already Initialized");
	}

	YAML::Node assetManifest = YAML::LoadFile(ASSET_MANIFEST_FILE);
	for (yaml_val& texture : assetManifest["textures"])
	{
		std::string key = texture["key"].as<std::string>();
		std::string path = "Assets/Textures/" + texture["path"].as<std::string>();
		Get().m_textureManifest[key] = path;
	}
}

sf::Texture* Xeph2D::AssetManager::GetTexture(const std::string& key)
{
	if (Get().m_loadedTextures.find(key) == Get().m_loadedTextures.end())
	{
		Debug::LogErr("AssetManager -> Could not find loaded texture with key: %s", key.c_str());
		return nullptr;
	}

	return Get().m_loadedTextures.at(key).get();
}

void Xeph2D::AssetManager::LoadTexture(const std::string& key)
{
	m_loadedTextures[key] = std::make_unique<sf::Texture>();
	m_loadedTextures[key]->loadFromFile(Get().m_textureManifest[key]);
}

void Xeph2D::AssetManager::ClearTextures()
{
	m_loadedTextures.clear();
}
