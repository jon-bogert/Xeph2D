#ifndef __X2DIS_EDITORIS_EDITORWINDOWS_SCRIPTMANAGER_H__
#define __X2DIS_EDITORIS_EDITORWINDOWS_SCRIPTMANAGER_H__
#ifdef IS_EDITOR

#include "Xeph2D/Editor/EditorWindow.h"

#include <map>
#include <vector>
#include <string>

namespace Xeph2D::Edit
{
	class ComponentManager : public EditorWindow
	{
	public:
		void Initialize() override;
		void OnGUI() override;

		std::string GetComponentName(uint32_t typeID) const;
		std::vector<std::string> GetAllNames();
		uint32_t GetID(std::string name) const;
	private:
		void LoadFromFile();
		void SaveToFile();
		void GenerateHeader();

		friend class ComponentCreator;
		void CreateNew(const std::string& name, std::string& path);
		bool EditComponent(uint32_t id);
		void GenerateFiles(const std::string& name, const std::string& path, uint32_t newID);

		struct Entry
		{
			std::string name;
			std::string path;

			Entry() = default;
			Entry(const std::string& name, const std::string& path) : name(name), path(path) {}
		};

		std::map<uint32_t, std::string> m_defaultComponents;
		std::map<uint32_t, Entry> m_userComponents;

		int m_editSelection = -1;

		bool m_isRemoving = false;
		bool m_isEditing = false;
		char m_nameBuffer[256];
		char m_pathBuffer[1024];
	};
}

#endif //IS_EDITOR
#endif //!__X2DIS_EDITORIS_EDITORWINDOWS_SCRIPTMANAGER_H__
