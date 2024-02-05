#ifndef __X2D_EDITOR_EDITORWINDOWS_VIEWPORT_H__
#define __X2D_EDITOR_EDITORWINDOWS_VIEWPORT_H__

#ifdef _EDITOR

#include <Xeph2D/Editor/EditorWindow.h>
#include <Xeph2D.h>

namespace Xeph2D::Edit
{
	class Viewport : public EditorWindow
	{
	public:
		void Initialize() override;
		void OnGUI() override;

		Vector2 GetMousePos() const;

	private:
		Xeph2D::Vector2 m_size;
		bool m_isHovered = false;
		Vector2 m_mouseInViewport{};
	};
}
#endif //_EDITOR
#endif //!__X2D_EDITOR_EDITORWINDOWS_VIEWPORT_H__