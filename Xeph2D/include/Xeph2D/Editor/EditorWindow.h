#ifndef __X2D_EDITOR_EDITORWINDOW_H__
#define __X2D_EDITOR_EDITORWINDOW_H__

#ifdef _EDITOR
#include <imgui.h>

#include <string>

namespace Xeph2D::Edit
{
	class EditorWindow
	{
		friend class Editor;

	public:
		EditorWindow() = default;
		virtual ~EditorWindow() = default;

		virtual void Initialize() {};
		virtual void OnGUI() {};

		const char* GetWindowName() const;
		//ImGuiWindowFlags GetFlags() const;
		bool IsOpen() const;
		void Close();
		void Open();

		bool IsFocused() const;
		bool IsHovered() const;

	protected:
		std::string name = "<Unnamed>";
		int flags = 0;
		bool isOpen = true;

	private:
		bool m_isFocused = false;
		bool m_isHovered = false;

		void UpdateValues();
	};
}

#endif //_EDITOR
#endif //!__X2D_EDITOR_EDITORWINDOW_H__