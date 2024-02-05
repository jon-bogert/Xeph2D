#ifndef __X2D_EDITOR_EDITORWINDOWS_HIERARCHY_H__
#define __X2D_EDITOR_EDITORWINDOWS_HIERARCHY_H__

#ifdef _EDITOR

#include <Xeph2D/Editor/EditorWindow.h>
#include <Xeph2D.h>

namespace Xeph2D::Edit
{
	class Hierarchy : public EditorWindow
	{
	public:
		void Initialize() override;
		void OnGUI() override;

	private:
		void ConstructNameList();

		int m_selectionIndex = -1;
		std::vector<std::string> m_objectNames;
	};
}
#endif //_EDITOR

#endif //!__X2D_EDITOR_EDITORWINDOWS_HIERARCHY_H__