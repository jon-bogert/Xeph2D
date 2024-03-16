#ifndef __X2D_SYSTEMS_ASSETMANAGER_H__
#define __X2D_SYSTEMS_ASSETMANAGER_H__

#include <SFML.hpp>
#include <unordered_map>
#include <memory>

namespace Xeph2D
{
#ifdef IS_EDITOR
	namespace Edit
	{
		class Editor;
		class AssetManagerWindow;
	}
#endif //IS_EDITOR
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
		using DataMap = std::unordered_map<std::string, std::unique_ptr<std::vector<uint8_t>>>;

		using Manifest = std::unordered_map<std::string, std::string>;

		friend class SceneManager;
#ifdef IS_EDITOR
		friend class Edit::Editor;
		friend class Edit::AssetManagerWindow;
		void SetIsSaved(const bool isSaved) { m_isSaved = isSaved; }
		void SaveToFile();
		bool m_isSaved = true;
#endif //IS_EDITOR
		void LoadTexture(const std::string& key);
		void UnloadTexture(const std::string& key);
		void ClearTextures();

		Manifest m_textureManifest;
		TextureMap m_loadedTextures;
#ifndef IS_DEBUG
		DataMap m_loadedTexturesData;
#endif // !IS_DEBUG
		std::unique_ptr<sf::Texture> m_defaultTexture;
		std::unique_ptr<unsigned char[]> m_defaultTextureData;
		size_t m_defaultTextureSize = 0;
	};
}

#endif //__X2D_SYSTEMS_ASSETMANAGER_H__
