#include "Xeph2D/Components/TextRenderer.h"

#include "Xeph2D/Systems/AssetManager.h"
#include "Xeph2D/Systems/RenderStack.h"
#include "Xeph2D/Systems/SceneManager.h"
#include "Xeph2D/Systems/Time.h"
#include "Xeph2D/Systems/WindowManager.h"

#include <functional>

void Xeph2D::TextRenderer::SetColor(const Color& color)
{
	m_color = color;
	m_text.setColor(color);
}

void Xeph2D::TextRenderer::SetFont(const std::string& key)
{
	sf::Font* font = AssetManager::GetFont(key);
	if (font == nullptr)
	{
		Debug::LogErr("TextRenderer -> Could not find font with key: %s", key.c_str());
		return;
	}
	m_fontKey = key;
	m_text.setFont(*font);
	Readjust();
}

void Xeph2D::TextRenderer::SetFontSize(const int size)
{
	m_fontSize = size;
	m_text.setCharacterSize(SizeCalc());
	Readjust();
}

void Xeph2D::TextRenderer::SetContents(const std::string& contents)
{
	m_contents = contents;
	m_text.setString(contents);
	Readjust();
}

void Xeph2D::TextRenderer::OnEditorStart()
{
#ifdef IS_EDITOR
	Awake();
	m_fontKeyPrev = m_fontKey;
	m_refreshTimer.SetLengthSeconds(m_timeToRefresh);
	m_refreshTimer.SetIsUnscaled(true);
#endif //IS_EDITOR
}

void Xeph2D::TextRenderer::OnEditorUpdate()
{
#ifdef IS_EDITOR
	if (m_contents != m_contentsPrev)
	{
		SetContents(m_contents);
	}
	m_contentsPrev = m_contents;

	if (m_pivot != m_pivotPrev)
	{
		Readjust();
	}
	m_pivotPrev = m_pivot;

	if (m_fontSize != m_fontSizePrev)
	{
		SetFontSize(m_fontSize);
	}

	if (m_refreshTimer.ExpiredThisFrame())
	{
		SetFont(m_fontKey);
	}
	if (m_fontKeyPrev == m_fontKey)
		return;

	m_refreshTimer.Restart();
	m_fontKeyPrev = m_fontKey;
#endif //IS_EDITOR
}

void Xeph2D::TextRenderer::OnEditorShutdown()
{
	OnDestroy();
}

void Xeph2D::TextRenderer::Serializables()
{
	SERIALIZE_BOOL(m_isUiElement);
	SERIALIZE_STRING(m_fontKey);
	SERIALIZE_STRING(m_contents);
	SERIALIZE_INT(m_fontSize);
	SERIALIZE_VEC2(m_pivot);
	SERIALIZE_COLOR(m_color);
}

void Xeph2D::TextRenderer::Awake()
{
	RenderStack::SubscribeDrawCall(this, std::bind(&TextRenderer::Draw, this));
	m_text.setCharacterSize(SizeCalc());
	m_text.setString(m_contents);
	SetFont(m_fontKey);
}

void Xeph2D::TextRenderer::OnDestroy()
{
	RenderStack::UnubscribeDrawCall(this);
}

void Xeph2D::TextRenderer::Draw()
{
#ifdef IS_EDITOR
	SetColor(m_color);
#endif //IS_EDITOR
	if (!IsActiveAndEnabled())
		return;
	if (m_isUiElement)
		RenderStack::AddUIDrawable(gameObject, &m_text, &m_text, m_order);
	else
		RenderStack::AddDrawable(gameObject, &m_text, &m_text, m_order);
}

void Xeph2D::TextRenderer::Readjust()
{
	Vector2 centerPoint = { m_text.getLocalBounds().width, m_text.getLocalBounds().height };
	m_pivot.x = Math::Clamp(m_pivot.x, -1.f, 1.f);
	m_pivot.y = Math::Clamp(m_pivot.y, -1.f, 1.f);

	Vector2 pivotAdjusted;
	pivotAdjusted.x = Math::Remap(-1.f, 1.f, 0.f, 1.f, m_pivot.x);
	pivotAdjusted.y = 1.f - Math::Remap(-1.f, 1.f, 0.f, 1.f, m_pivot.y);

	centerPoint.x *= pivotAdjusted.x;
	centerPoint.y *= pivotAdjusted.y;
	m_text.setOrigin(centerPoint);
}

int Xeph2D::TextRenderer::SizeCalc()
{
	if (m_fontSize < 0)
		m_fontSize = 0;

	int adjusted = m_fontSize;
	//adjusted *= WindowManager::ResolutionScale(); // TODO - Use this instead to adjust size instead of RenderStack

	return adjusted;
}
