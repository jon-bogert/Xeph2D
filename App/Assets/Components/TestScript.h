#pragma once

#include <Xeph2D.h>

class TestScript : public Xeph2D::Component
{
	XEPH2D_REG_COMP(0xa45ff78b);
private:
	void Serializables() override;

	void OnEditorStart() override;

	void Start() override;
	void Update() override;
	void OnDestroy() override;

	Xeph2D::Color m_colors[2] = { Xeph2D::Color::White, Xeph2D::Color::Red };
	int m_colorIndex = 0;

	Xeph2D::Ref<Xeph2D::SpriteRenderer> m_spriteRenderer;
	std::vector<Xeph2D::Ref<Xeph2D::GameObject>> m_others;

	bool m_isMain = false;

};