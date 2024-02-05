#include "Xeph2D/Components/SpriteRenderer.h"

#include "Xeph2D/Systems/AssetManager.h"
#include "Xeph2D/Systems/RenderStack.h"
#include "Xeph2D/Systems/SceneManager.h"
#include "Xeph2D/Systems/Time.h"

#include <functional>

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
}

void Xeph2D::SpriteRenderer::Awake()
{
	RenderStack::SubscribeDrawCall(this, std::bind(&SpriteRenderer::Draw, this));

	sf::Texture* texture = AssetManager::GetTexture(m_textureKey);
	m_tempSprite.setTexture(*texture);
	m_tempSprite.setOrigin(Vector2(texture->getSize()) * 0.5f);
}

void Xeph2D::SpriteRenderer::OnDestroy()
{
	RenderStack::UnubscribeDrawCall(this);
}

void Xeph2D::SpriteRenderer::Draw()
{
	RenderStack::AddDrawable(gameObject, &m_tempSprite, &m_tempSprite, m_order);
}
