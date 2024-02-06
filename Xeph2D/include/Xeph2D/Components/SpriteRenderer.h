#ifndef __XEPH2D_COMPONENTS_SPRITERENDERER_H__
#define __XEPH2D_COMPONENTS_SPRITERENDERER_H__

#include "Xeph2D/Component.h"

namespace Xeph2D
{
	class SpriteRenderer : public Component
	{
		XEPH2D_REG_COMP(0x00000002);
	public:
		int GetOrder() const { return m_order; }
		void SetOrder(const int order) { m_order = order; }

		Color GetColor() const { return m_color; }
		void SetColor(const Color& color);

	private:
		void OnEditorStart() override;
		void OnEditorShutdown() override;

		void Serializables() override;
		void Awake() override;
		void OnDestroy() override;

		void Draw();

		sf::Sprite m_sprite;

		std::string m_textureKey = "";
		int m_order = 0;
		Color m_color = Color::White;
	};
}

#endif //!__XEPH2D_COMPONENTS_SPRITERENDERER_H__
