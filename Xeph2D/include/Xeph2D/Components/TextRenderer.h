#ifndef __XEPH2D_COMPONENTS_TEXTRENDERER_H__
#define __XEPH2D_COMPONENTS_TEXTRENDERER_H__

#include "Xeph2D/Component.h"
#include "Xeph2D/Timer.h"

namespace Xeph2D
{
	class TextRenderer : public Component
	{
		XEPH2D_REG_COMP(0x00000002);
	public:
		int GetOrder() const { return m_order; }
		void SetOrder(const int order) { m_order = order; }

		Color GetColor() const { return m_color; }
		void SetColor(const Color& color);

		std::string GetFontKey() const { return m_fontKey; }
		void SetFont(const std::string& key);

		int GetFontSize() const { return m_fontSize; }
		void SetFontSize(const int size);

		std::string GetContents() const { return m_contents; }
		void SetContents(const std::string& contents);

	private:
		void OnEditorStart() override;
		void OnEditorUpdate() override;
		void OnEditorShutdown() override;

		void Serializables() override;
		void Awake() override;
		void OnDestroy() override;

		void Draw();

		void Readjust();
		int SizeCalc();

		sf::Text m_text;

		bool m_isUiElement = false;
		std::string m_fontKey = "";
		Vector2 m_pivot = Vector2::Zero();
		std::string m_contents = "Lorum Ipsum";
		int m_fontSize = 32;
		int m_order = 0;
		Color m_color = Color::White;

#ifdef IS_EDITOR
		float m_timeToRefresh = 1.f;
		Timer m_refreshTimer;
		std::string m_fontKeyPrev = "";
		std::string m_contentsPrev = "Lorum Ipsum";
		Vector2 m_pivotPrev = Vector2::Zero();
		int m_fontSizePrev = 32;
#endif //IS_EDITOR
	};
}

#endif //__XEPH2D_COMPONENTS_TEXTRENDERER_H__