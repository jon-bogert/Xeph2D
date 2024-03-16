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

void Xeph2D::SpriteRenderer::SetTexture(const std::string& key)
{
	sf::Texture* texture = AssetManager::GetTexture(key);
	if (texture == nullptr)
	{
		Debug::LogErr("SpriteRenderer -> Could not find texture with key: %s", key.c_str());
		return;
	}
	m_textureKey = key;
	m_sprite.setTexture(*texture);
	m_sprite.setOrigin(Vector2(texture->getSize()) * 0.5f);
}

void Xeph2D::SpriteRenderer::OnEditorStart()
{
#ifdef IS_EDITOR
	Awake();
	m_textureKeyPrev = m_textureKey;
	m_refreshTimer.SetLengthSeconds(m_timeToRefresh);
	m_refreshTimer.SetIsUnscaled(true);
#endif //IS_EDITOR
}

void Xeph2D::SpriteRenderer::OnEditorUpdate()
{
#ifdef IS_EDITOR
	if (m_refreshTimer.ExpiredThisFrame())
	{
		SetTexture(m_textureKey);
	}
	if (m_textureKeyPrev == m_textureKey)
		return;

	m_refreshTimer.Restart();
	m_textureKeyPrev = m_textureKey;
#endif //IS_EDITOR
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
	SetTexture(m_textureKey);
}

void Xeph2D::SpriteRenderer::OnDestroy()
{
	RenderStack::UnubscribeDrawCall(this);
}

void Xeph2D::SpriteRenderer::Draw()
{
#ifdef IS_EDITOR
	SetColor(m_color);
#endif //IS_EDITOR
	if (IsActiveAndEnabled())
		RenderStack::AddDrawable(gameObject, &m_sprite, &m_sprite, m_order);
}
