#ifndef __X2DIS_EDITORIS_EDITORWINDOWS_VIEWPORT_H__
#define __X2DIS_EDITORIS_EDITORWINDOWS_VIEWPORT_H__

#ifdef IS_EDITOR

#include <Xeph2D/Editor/EditorWindow.h>
#include <Xeph2D.h>

namespace Xeph2D::Edit
{
	class Viewport : public EditorWindow
	{
	public:
		void Initialize() override;
		void OnGUI() override;

		void DebugUI();

		Vector2 GetMousePos() const;

	private:
#ifdef IS_EDITOR
		friend class ProjectSettings;
		void UpdateSize() { m_updateSize = true; }
#endif //IS_EDITOR

		Xeph2D::Vector2 m_size;
		bool m_isHovered = false;
		Vector2 m_mouseInViewport{};

		bool m_updateSize = false;
	};
}
#endif //IS_EDITOR
#endif //!__X2DIS_EDITORIS_EDITORWINDOWS_VIEWPORT_H__