#ifdef IS_EDITOR
#include "Xeph2D/Editor/Editor.h"
#include "Xeph2D/Editor/TransformGizmo.h"
#include "Xeph2D/Systems/Debug.h"
#include "Xeph2D/Systems/WindowManager.h"

#include <cmath>

#define __ROT_SPEED 3.f
#define __POSITION transform.As<Transform>().position
#define __ROTATION transform.As<Transform>().rotation
#define __SCALE transform.As<Transform>().scale

using namespace Xeph2D::Edit;

void Xeph2D::Edit::TransformGizmo::UpdateMouse(const Vector2& mousePos)
{
	if (m_currentObject == nullptr)
		return;

	Vector2 position = WindowManager::WorldToPixel(m_currentObject->__POSITION);

	if (!m_applyingTransform && (m_mode == Mode::Position || m_mode == Mode::Scale))
	{
		//both Square
		if (mousePos.x >= position.x && mousePos.x <= position.x + m_centerWidth &&
			mousePos.y <= position.y && mousePos.y >= position.y - m_centerWidth)
		{
			m_xSelected = true;
			m_ySelected = true;
		}
		// X line
		else if (mousePos.x >= position.x + m_centerWidth && mousePos.x <= position.x + m_radius + m_symbolWidth &&
			mousePos.y <= position.y + m_centerWidth * 0.5f && mousePos.y >= position.y - m_centerWidth * 0.5f)
		{
			m_xSelected = true;
			m_ySelected = false;
		}
		// Y line
		else if (mousePos.x <= position.x + m_centerWidth * 0.5f && mousePos.x >= position.x - m_centerWidth * 0.5f &&
			mousePos.y <= position.y - m_centerWidth && mousePos.y >= position.y - m_radius - m_symbolWidth)
		{
			m_xSelected = false;
			m_ySelected = true;
		}
		else
		{
			m_xSelected = false;
			m_ySelected = false;
		}
	}
	else if (m_mode == Mode::Rotation)
	{
		float mag = Math::Magnitude(position - mousePos);
		if (mag < m_radius + m_symbolWidth && mag > m_radius - m_symbolWidth)
		{
			m_xSelected = true;
			m_ySelected = true;
		}
		else
		{
			m_xSelected = false;
			m_ySelected = false;
		}
	}

	if ((m_xSelected || m_ySelected) &&
		(InputSystem::GetMouseDown(Mouse::Button::Left) && !InputSystem::GetKeyHold(Key::LAlt)))
	{
		m_applyingTransform = true;
	}
	else if (InputSystem::GetMouseUp(Mouse::Button::Left))
	{
		m_applyingTransform = false;
	}

	ApplyTransform();
}

void TransformGizmo::Draw()
{
	if (m_currentObject == nullptr)
		return;

	Debug::VertexChain vertChain;
	Vector2 out;
	Vector2 cwUnit = { WindowManager::PixelToUnit(m_centerWidth), WindowManager::PixelToUnit(m_centerWidth) };
	float swUnit = WindowManager::PixelToUnit(m_symbolWidth);
	Color xColor = (m_xSelected) ? Color::Yellow : Color::Red;
	Color yColor = (m_ySelected) ? Color::Yellow : Color::Green;
	Color zColor = (m_xSelected || m_ySelected) ? Color::Yellow : Color::Blue;
	Color bothColor = (m_xSelected && m_ySelected) ? Color::Yellow : Color::Blue;
	bothColor.a = 0.25f;

	switch (m_mode)
	{
	case Mode::Position:
		vertChain.push_back(m_currentObject->__POSITION);
		out = WindowManager::PixelToUnit({ m_radius, 0.f });
		vertChain.push_back(m_currentObject->__POSITION + out);
		vertChain.push_back(m_currentObject->__POSITION + out + Vector2(0.f, swUnit * 0.5f));
		vertChain.push_back(m_currentObject->__POSITION + out + Vector2(swUnit, 0.f));
		vertChain.push_back(m_currentObject->__POSITION + out + Vector2(0.f, -swUnit * 0.5f));
		vertChain.push_back(m_currentObject->__POSITION + out);
		Debug::DrawChainLine(vertChain, xColor);

		vertChain.clear();
		vertChain.push_back(m_currentObject->__POSITION);
		out = WindowManager::PixelToUnit({ 0.f, m_radius });
		vertChain.push_back(m_currentObject->__POSITION + out);
		vertChain.push_back(m_currentObject->__POSITION + out + Vector2(-swUnit * 0.5f, 0.f));
		vertChain.push_back(m_currentObject->__POSITION + out + Vector2(0.f, swUnit));
		vertChain.push_back(m_currentObject->__POSITION + out + Vector2(swUnit * 0.5f, 0.f));
		vertChain.push_back(m_currentObject->__POSITION + out);
		Debug::DrawChainLine(vertChain, yColor);

		Debug::DrawBoxFilled(m_currentObject->__POSITION + cwUnit * 0.5f, cwUnit, 0.f, bothColor);
		break;
	case Mode::Rotation:
		Debug::DrawCircleOutline(m_currentObject->__POSITION, WindowManager::PixelToUnit(m_radius), zColor);
		out = Vector2(std::cosf(m_currentObject->__ROTATION.GetRad()), -std::sinf(m_currentObject->__ROTATION.GetRad())) * WindowManager::PixelToUnit(m_radius);
		Debug::DrawLine(m_currentObject->__POSITION, m_currentObject->__POSITION + out, zColor);
		break;
	case Mode::Scale:
		vertChain.push_back(m_currentObject->__POSITION);
		out = WindowManager::PixelToUnit({ m_radius, 0.f });
		vertChain.push_back(m_currentObject->__POSITION + out);
		vertChain.push_back(m_currentObject->__POSITION + out + Vector2(0.f, swUnit * 0.5f));
		vertChain.push_back(m_currentObject->__POSITION + out + Vector2(swUnit, swUnit * 0.5f));
		vertChain.push_back(m_currentObject->__POSITION + out + Vector2(swUnit, -swUnit * 0.5f));
		vertChain.push_back(m_currentObject->__POSITION + out + Vector2(0.f, -swUnit * 0.5f));
		vertChain.push_back(m_currentObject->__POSITION + out);
		Debug::DrawChainLine(vertChain, xColor);

		vertChain.clear();
		vertChain.push_back(m_currentObject->__POSITION);
		out = WindowManager::PixelToUnit({ 0.f, m_radius });
		vertChain.push_back(m_currentObject->__POSITION + out);
		vertChain.push_back(m_currentObject->__POSITION + out + Vector2(-swUnit * 0.5f, 0.f));
		vertChain.push_back(m_currentObject->__POSITION + out + Vector2(-swUnit * 0.5f, swUnit));
		vertChain.push_back(m_currentObject->__POSITION + out + Vector2(swUnit * 0.5f, swUnit));
		vertChain.push_back(m_currentObject->__POSITION + out + Vector2(swUnit * 0.5f, 0.f));
		vertChain.push_back(m_currentObject->__POSITION + out);
		Debug::DrawChainLine(vertChain, yColor);

		Debug::DrawBoxFilled(m_currentObject->__POSITION + cwUnit * 0.5f, cwUnit, 0.f, bothColor);
		break;
	default:
		Debug::LogErr("TransfromGizmo::Draw -> Enum type not supported");
		break;
	}
}

void TransformGizmo::SetCurrentObject(EditorGameObject* gameObject)
{
	m_currentObject = gameObject;
}

TransformGizmo::Mode TransformGizmo::GetMode() const
{
	return m_mode;
}

void TransformGizmo::SetMode(const Mode mode)
{
	m_mode = mode;
}

void Xeph2D::Edit::TransformGizmo::ApplyTransform()
{
	if (!m_applyingTransform)
		return;

	Vector2 mouseDelta{};
	InputSystem::GetMouseDelta(&mouseDelta.x);
	mouseDelta = WindowManager::PixelToUnit(mouseDelta);
	mouseDelta.y *= -1;

	float scaleBoth = (mouseDelta.x + mouseDelta.y) * 0.5f;
	Vector2 globalMousePos{};

	switch (m_mode)
	{
	case Mode::Position:
		if (m_xSelected)
			m_currentObject->__POSITION.x += mouseDelta.x;
		if (m_ySelected)
			m_currentObject->__POSITION.y += mouseDelta.y;
		break;
	case Mode::Rotation:
		InputSystem::GetMousePos(&globalMousePos.x, false);
		m_currentObject->__ROTATION.AddDeg(mouseDelta.x * __ROT_SPEED);
		break;
	case Mode::Scale:
		if (m_xSelected && m_ySelected)
		{
			m_currentObject->__SCALE += Vector2(scaleBoth, scaleBoth);
			break;
		}
		if (m_xSelected)
			m_currentObject->__SCALE.x += mouseDelta.x;
		if (m_ySelected)
			m_currentObject->__SCALE.y += mouseDelta.y;
		break;
	}
	Editor::SetIsSaved(false);
}

#endif // IS_EDITOR


