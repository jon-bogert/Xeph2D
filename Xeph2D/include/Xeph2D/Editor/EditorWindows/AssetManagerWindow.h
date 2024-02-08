#ifndef __X2D_EDITOR_EDITORWINDOWS_ASSETMANAGERWINDOW_H__
#define __X2D_EDITOR_EDITORWINDOWS_ASSETMANAGERWINDOW_H__

#ifdef _EDITOR
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
		enum class Type { Texture, Audio, Font };
		struct Entry
		{
			std::string key;
			std::string path;
			bool isInScene = false;

			Entry() = default;
			inline Entry(const std::string& key, const std::string& path, const bool isInScene) : key(key), path(path), isInScene(isInScene) {}
		};

		void AddTexture();

		std::vector<Entry> m_textures;
		std::vector<Entry> m_audio;
		std::vector<Entry> m_fonts;

		Type m_editType = Type::Texture;
		int m_editIndex = -1;
		bool m_showOptions = false;
		ImVec2 m_optionsPos;
		bool m_confirmRemove;
	};
}

#endif //_EDITOR
#endif //!__X2D_EDITOR_EDITORWINDOWS_ASSETMANAGERWINDOW_H__
