#ifndef __X2DIS_EDITORIS_EDITORWINDOWS_ASSETMANAGERWINDOW_H__
#define __X2DIS_EDITORIS_EDITORWINDOWS_ASSETMANAGERWINDOW_H__

#ifdef IS_EDITOR
#include "Xeph2D/Editor/EditorWindow.h"

#include <string>
#include <vector>

namespace Xeph2D::Edit
{
	class AssetManagerWindow : public EditorWindow
	{
	public:
		void Initialize() override;
		void OnGUI() override;

	private:
		enum class Type { Texture, Font, Audio };
		struct Entry
		{
			std::string key;
			std::string path;
			bool isInScene = false;

			Entry() = default;
			inline Entry(const std::string& key, const std::string& path, const bool isInScene) : key(key), path(path), isInScene(isInScene) {}
		};

		void AddTexture();
		void AddFont();

		std::vector<Entry> m_textures;
		std::vector<Entry> m_audio;
		std::vector<Entry> m_fonts;

		Type m_editType = Type::Texture;
		int m_editIndex = -1;
		bool m_showOptions = false;
		ImVec2 m_optionsPos;
		bool m_confirmRemove;
		bool m_isEditing;
		char m_keyBuffer[256];
	};
}

#endif //IS_EDITOR
#endif //!__X2DIS_EDITORIS_EDITORWINDOWS_ASSETMANAGERWINDOW_H__
