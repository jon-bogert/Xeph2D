#ifndef __XEPH2D_COMPONENTS_SPRITERENDERER_H__
#define __XEPH2D_COMPONENTS_SPRITERENDERER_H__

#include "Xeph2D/Component.h"
#include "Xeph2D/Timer.h"

namespace Xeph2D
{
	class SpriteRenderer : public Component
	{
		friend class SpritesheetRenderer;
		XEPH2D_REG_COMP(0x00000002);
	public:
		int GetOrder() const { return m_order; }
		void SetOrder(const int order) { m_order = order; }

		Color GetColor() const { return m_color; }
		void SetColor(const Color& color);

		std::string GetTextureKey() const { return m_textureKey; }
		void SetTexture(const std::string& key);

	private:
		void OnEditorStart() override;
		void OnEditorUpdate() override;
		void OnEditorShutdown() override;

		void Serializables() override;
		void Awake() override;
		void OnDestroy() override;

		void Draw();

		sf::Sprite m_sprite;

		std::string m_textureKey = "";
		int m_order = 0;
		Color m_color = Color::White;

#ifdef IS_EDITOR
		float m_timeToRefresh = 1.f;
		Timer m_refreshTimer;
		std::string m_textureKeyPrev = "";
#endif //IS_EDITOR
	};
}

#endif //!__XEPH2D_COMPONENTS_SPRITERENDERER_H__
