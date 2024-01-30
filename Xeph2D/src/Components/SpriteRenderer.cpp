#include "Xeph2D/Components/SpriteRenderer.h"

#include "Xeph2D/Systems/RenderStack.h"
#include "Xeph2D/Systems/Time.h"

#include <functional>

void Xeph2D::SpriteRenderer::Awake()
{
	RenderStack::SubscribeDrawCall(this, std::bind(&SpriteRenderer::Draw, this));

	m_tempTex.loadFromFile("Assets/Textures/test2.png");
	m_tempSprite.setTexture(m_tempTex);
	m_tempSprite.setOrigin(Vector2(m_tempTex.getSize()) * 0.5f);
}

void Xeph2D::SpriteRenderer::OnDestroy()
{
	RenderStack::UnubscribeDrawCall(this);
}

void Xeph2D::SpriteRenderer::Draw()
{
	RenderStack::AddDrawable(gameObject, &m_tempSprite, &m_tempSprite, m_order);
}
