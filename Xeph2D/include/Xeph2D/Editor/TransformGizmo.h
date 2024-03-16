#ifndef __X2DIS_EDITOR_TRANSFORMGIZMO_H__
#define __X2DIS_EDITOR_TRANSFORMGIZMO_H__
#ifdef IS_EDITOR

namespace Xeph2D
{
	class GameObject;
	struct Vector2;
}

namespace Xeph2D::Edit
{
	struct EditorGameObject;
	class TransformGizmo
	{
	public:
		enum class Mode { Position, Rotation, Scale };
		void UpdateMouse(const Vector2& mousePos);
		void Draw();
		void SetCurrentObject(EditorGameObject* gameObject);

		Mode GetMode() const;
		void SetMode(const Mode mode);

	private:
		void ApplyTransform();

		Mode m_mode = Mode::Position;
		float m_radius = 100.f;
		float m_symbolWidth = 15.f;
		float m_centerWidth = 25.f;

		bool m_xSelected = false;
		bool m_ySelected = false;

		bool m_applyingTransform = false;

		EditorGameObject* m_currentObject = nullptr;
	};
}

#endif //IS_EDITOR
#endif //!__X2DIS_EDITOR_TRANSFORMGIZMO_H__