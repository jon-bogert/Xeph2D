#ifndef __XEPH2D_COMPONENTS_SPRITESHEETRENDERER_H__
#define __XEPH2D_COMPONENTS_SPRITESHEETRENDERER_H__

#include "Xeph2D/Component.h"
#include "Xeph2D/Structs.h"

#include "Xeph2D/Components/SpriteRenderer.h"

namespace Xeph2D
{
	class SpritesheetRenderer : public Component
	{
		XEPH2D_REG_COMP(0x00000003);
	public:
		void SetCell(const Vector2& cell);
		Vector2 GetCell() const { return m_cell; }

	private:
		void Serializables() override;
		void Awake() override;

		void OnEditorStart() override;

		Vector2 m_cell = Vector2::Zero(); //current cell (in cells)
		Vector2 m_size = Vector2::Zero(); //size of each cell (in pixels)
		Vector2 m_padding = Vector2::Zero(); // size of padding (in pixels)

		Ref<SpriteRenderer> m_renderer;
	};
}

#endif //!__XEPH2D_COMPONENTS_SPRITESHEETRENDERER_H__