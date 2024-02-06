#include "Xeph2D/Components/SpriteRenderer.h"

#include "Xeph2D/Systems/AssetManager.h"
#include "Xeph2D/Systems/RenderStack.h"
#include "Xeph2D/Systems/SceneManager.h"
#include "Xeph2D/Systems/Time.h"

#include <functional>

void Xeph2D::SpriteRenderer::SetColor(const Color& color)
{
	m_color = color;
	m_sprite.setColor(color);
}

void Xeph2D::SpriteRenderer::OnEditorStart()
{
	Awake();
}

void Xeph2D::SpriteRenderer::OnEditorShutdown()
{
	OnDestroy();
}

void Xeph2D::SpriteRenderer::Serializables()
{
	SERIALIZE_STRING(m_textureKey);
	SERIALIZE_COLOR(m_color);
}

void Xeph2D::SpriteRenderer::Awake()
{
	RenderStack::SubscribeDrawCall(this, std::bind(&SpriteRenderer::Draw, this));

	sf::Texture* texture = AssetManager::GetTexture(m_textureKey);
	m_sprite.setTexture(*texture);
	m_sprite.setOrigin(Vector2(texture->getSize()) * 0.5f);
}

void Xeph2D::SpriteRenderer::OnDestroy()
{
	RenderStack::UnubscribeDrawCall(this);
}

void Xeph2D::SpriteRenderer::Draw()
{
#ifdef _EDITOR
	SetColor(m_color);
#endif //_EDITOR
	if (IsActiveAndEnabled())
		RenderStack::AddDrawable(gameObject, &m_sprite, &m_sprite, m_order);
}
