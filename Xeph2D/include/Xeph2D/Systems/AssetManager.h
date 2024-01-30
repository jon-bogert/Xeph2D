#ifndef __X2D_SYSTEMS_ASSETMANAGER_H__
#define __X2D_SYSTEMS_ASSETMANAGER_H__

#include <SFML.hpp>
#include <unordered_map>
#include <memory>

namespace Xeph2D
{
	class AssetManager
	{
	public:
		~AssetManager() = default;
		AssetManager(const AssetManager& other) = delete;
		AssetManager(const AssetManager&& other) = delete;
		AssetManager operator=(const AssetManager& other) = delete;
		AssetManager operator=(const AssetManager&& other) = delete;

		static void Initialize();
		static sf::Texture* GetTexture(const std::string& key);

	private:
		AssetManager() {}
		static AssetManager& Get() { static AssetManager instance; return instance; }
		using TextureMap = std::unordered_map<std::string, std::unique_ptr<sf::Texture>>;
		using FontMap = std::unordered_map<std::string, std::unique_ptr<sf::Font>>;

		using Manifest = std::unordered_map<std::string, std::string>;

		friend class SceneManager;
		void LoadTexture(const std::string& key);
		void ClearTextures();

		Manifest m_textureManifest;
		TextureMap m_loadedTextures;
	};
}

#endif __X2D_SYSTEMS_ASSETMANAGER_H__
