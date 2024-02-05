#ifdef _EDITOR
#include "Xeph2D/Editor/Editor.h"

#define EDITOR_WINDOW_TITLE "Xeph2D Editor"

bool Xeph2D::Edit::Editor::IsOpen()
{
	return Get().m_window->isOpen();
}

void Xeph2D::Edit::Editor::Close()
{
	Get().m_window->close();
}

void Xeph2D::Edit::Editor::Initialize(
	std::function<std::unordered_map<uint32_t, std::string>(void)>& compNameCallback)
{
	Get().m_componentNames = compNameCallback();

	Get().m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1920, 1080), EDITOR_WINDOW_TITLE);
	Get().m_handle = FindWindowA(NULL, EDITOR_WINDOW_TITLE);
}

void Xeph2D::Edit::Editor::Update()
{
}

void Xeph2D::Edit::Editor::OnGUI()
{
}

void Xeph2D::Edit::Editor::Draw()
{
}

void Xeph2D::Edit::Editor::Terminate()
{
}

#endif //_EDITOR
