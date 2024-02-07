#ifndef __X2D_EDITOR_EDITORWINDOWS_SCRIPTMANAGER_H__
#define __X2D_EDITOR_EDITORWINDOWS_SCRIPTMANAGER_H__
#ifdef _EDITOR

#include "Xeph2D/Editor/EditorWindow.h"

#include <map>

namespace Xeph2D::Edit
{
	class ScriptManager : public EditorWindow
	{
	public:
		void Initialize() override;
		void OnGUI() override;

		std::string GetScriptName(uint32_t typeID) const;

	private:
		void LoadFromFile();
		void GenerateHeader();

		std::map<uint32_t, std::string> m_defaultScripts;
		std::map<uint32_t, std::string> m_userScripts;
	};
}

#endif //_EDITOR
#endif //!__X2D_EDITOR_EDITORWINDOWS_SCRIPTMANAGER_H__
