#ifndef __XEPH2D_COMPONENTS_SPRITERENDERER_H__
#define __XEPH2D_COMPONENTS_SPRITERENDERER_H__

#include "Xeph2D/Component.h"

namespace Xeph2D
{
	class SpriteRenderer : public Component
	{
		XEPH2D_REG_COMP(0x00000002);

		void Serializables() override;
		void Awake() override;
		void OnDestroy() override;

		void Draw();

		sf::Sprite m_tempSprite;

		std::string m_textureKey = "";
		int m_order = 0;
	};
}

#endif //!__XEPH2D_COMPONENTS_SPRITERENDERER_H__
