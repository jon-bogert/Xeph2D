#ifdef IS_EDITOR
#include "Xeph2D/Editor/EditorWindows/Viewport.h"
#include "Xeph2D/Systems/WindowManager.h"
#include <imgui-SFML.h>

using namespace Xeph2D;

void Xeph2D::Edit::Viewport::Initialize()
{
	name = "Viewport";
}

void Xeph2D::Edit::Viewport::OnGUI()
{
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - WindowManager::__Viewport()->getSize().x) * 0.5f);
	ImGui::SetCursorPosY((ImGui::GetWindowSize().y - WindowManager::__Viewport()->getSize().y + 20) * 0.5f);

	ImVec2 windowPos = ImGui::GetWindowPos(); // get the position of the current window
	ImVec2 mousePos = ImGui::GetMousePos(); // get the global mouse position
	m_mouseInViewport = ImVec2(mousePos.x - windowPos.x - ImGui::GetCursorPosX(), mousePos.y - windowPos.y - ImGui::GetCursorPosY()); // compute the mouse position relative to the window

	ImGui::Image(*WindowManager::__Viewport());

	if (Vector2(ImGui::GetWindowSize()) != m_size || m_updateSize)
	{
		m_updateSize = false;
		m_size = ImGui::GetWindowSize();
		WindowManager::__UpdateViewportSize(m_size - Vector2(10.f, 30.f));
	}
}

void Xeph2D::Edit::Viewport::DebugUI()
{
	//TODO Guard against min max of longlong
	Vector2 x0_min = Vector2(WindowManager::WorldWindowMinimum().x, 0);
	Vector2 x0_max = Vector2(WindowManager::WorldWindowMaximum().x, 0);
	Vector2 y0_min = Vector2(0, WindowManager::WorldWindowMinimum().y);
	Vector2 y0_max = Vector2(0, WindowManager::WorldWindowMaximum().y);

	Color worldOriginColor = { 0.75, 0.75, 0.75, 1.f };
	Color worldUnitColor = { 0.2, 0.2, 0.2, 1.f };

	Debug::DrawLine(x0_min, x0_max, worldOriginColor);
	Debug::DrawLine(y0_min, y0_max, worldOriginColor);

	//X-Lines
	for (long long y = (long long)WindowManager::WorldWindowMinimum().y;
		y < WindowManager::WorldWindowMaximum().y; ++y)
	{
		if (y == 0)
			continue;
		Vector2 min = Vector2(WindowManager::WorldWindowMinimum().x, y);
		Vector2 max = Vector2(WindowManager::WorldWindowMaximum().x, y);
		Debug::DrawLine(min, max, worldUnitColor);
	}
	//Y-Lines
	for (long long x = (long long)WindowManager::WorldWindowMinimum().x;
		x < WindowManager::WorldWindowMaximum().x; ++x)
	{
		if (x == 0)
			continue;
		Vector2 min = Vector2(x, WindowManager::WorldWindowMinimum().y);
		Vector2 max = Vector2(x, WindowManager::WorldWindowMaximum().y);
		Debug::DrawLine(min, max, worldUnitColor);
	}
}

Vector2 Xeph2D::Edit::Viewport::GetMousePos() const
{
	return m_mouseInViewport;
}

#endif //IS_EDITOR