#ifndef __X2D_EDITOR_EDITORWINDOWS_INSPECTOR_H__
#define __X2D_EDITOR_EDITORWINDOWS_INSPECTOR_H__

#ifdef _EDITOR

#include <Xeph2D/Editor/EditorWindow.h>
#include <Xeph2D.h>

namespace Xeph2D::Edit
{
	class Inspector : public EditorWindow
	{
	public:
		void Initialize() override;
		void OnGUI() override;

	private:
		friend class Hierarchy;
		void SetActiveIndex(int index);

		void ShowData(Field& field, uint32_t compID, bool showName = true, SerializableType type = SerializableType::Undefined); // Undefined will get type from field;
		std::string Var2DisplayName(std::string varName);

		int m_activeIndex = -1;
	};
}
#endif //_EDITOR

#endif //!__X2D_EDITOR_EDITORWINDOWS_INSPECTOR_H__