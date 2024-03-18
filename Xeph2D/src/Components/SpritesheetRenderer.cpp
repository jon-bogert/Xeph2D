#include "Xeph2D/Components/SpritesheetRenderer.h"

void Xeph2D::SpritesheetRenderer::SetCell(const Vector2& cell)
{
	// TODO - cast all values 
	uint32_t cellX = static_cast<uint32_t>(cell.x);
	uint32_t cellY = static_cast<uint32_t>(cell.y);
	int offX = cellX * m_size.x + cellX * m_padding.x;
	int offY = cellY * m_size.y + cellY * m_padding.y;

	m_renderer->m_sprite.setTextureRect({offX, offY, static_cast<int>(m_size.x), static_cast<int>(m_size.y) });
	m_renderer->m_sprite.setOrigin(m_size.x * 0.5f, m_size.y * 0.5f);
}

void Xeph2D::SpritesheetRenderer::Serializables()
{
	SERIALIZE_VEC2(m_cell);
	SERIALIZE_VEC2(m_size);
	SERIALIZE_VEC2(m_padding);
}

void Xeph2D::SpritesheetRenderer::Awake()
{
	m_renderer = GetComponent<SpriteRenderer>();
	if (m_renderer.IsNull())
	{
		Debug::LogErr("Spritesheet Renderer could not find Sprite Renderer Component");
		return;
	}

	SetCell(m_cell);
}

void Xeph2D::SpritesheetRenderer::OnEditorStart()
{
#if IS_EDITOR
	Awake();
#endif //IS_EDITOR
}
