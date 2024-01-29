/* ==================================================================================
* MIT License
*
* Copyright (c) 2023 Jon Bogert
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
================================================================================== */

#include "Xeph2D/Systems/InputSystem/InputSystem.h"

#include "GamepadHandler.h"
#include "KeyHandler.h"

using namespace Xeph2D;

namespace
{
	POINT WindowCenter(HWND hwnd)
	{
		POINT pnt;
		RECT windowRect;
		GetWindowRect(hwnd, &windowRect);
		RECT clientRect;
		GetClientRect(hwnd, &clientRect);
		int halfTitleBarHeight = (windowRect.bottom - windowRect.top - clientRect.bottom - clientRect.top) * 0.5f;
		// Calculate the center position
		pnt.x = (int)(windowRect.left + (windowRect.right - windowRect.left) * 0.5f);
		pnt.y = (int)(windowRect.top + (windowRect.bottom - windowRect.top) * 0.5f + halfTitleBarHeight);
		return pnt;
	}
}

Xeph2D::InputSystem::InputSystem()
{
	m_gamepadHandler = new GamepadHandler();
	m_keyHandler = new KeyHandler();
	GetCursorPos(&m_mousePos);
}

InputSystem& Xeph2D::InputSystem::Get()
{
	static InputSystem inst;
	return inst;
}

Xeph2D::InputSystem::~InputSystem()
{
	delete m_keyHandler;
	delete m_gamepadHandler;
}

//================================================================================
//                       STATIC INTERFACES
//================================================================================


void Xeph2D::InputSystem::Initialize(HWND& hwnd, std::function<void(InputSystem*)> func)
{
	Get()._Initialize(hwnd);
	func(&Get());
}

void Xeph2D::InputSystem::Update()
{
	Get()._Update();
}

bool Xeph2D::InputSystem::IsControllerConnected(const uint8_t num)
{
	return Get().m_gamepadHandler->IsControllerConnected(num);
}

bool Xeph2D::InputSystem::GetGamepadHold(Gamepad::Button button, uint8_t num)
{
	return Get().m_gamepadHandler->GetGamepadHold(button, num);
}

bool Xeph2D::InputSystem::GetGamepadDown(Gamepad::Button button, uint8_t num)
{
	return Get().m_gamepadHandler->GetGamepadDown(button, num);
}

bool Xeph2D::InputSystem::GetGamepadUp(Gamepad::Button button, uint8_t num)
{
	return Get().m_gamepadHandler->GetGamepadUp(button, num);
}

void Xeph2D::InputSystem::GetGamepadAxis(float* out_v2, Gamepad::Axis axis, uint8_t num)
{
	Get().m_gamepadHandler->GetGamepadAxis(out_v2, axis, num);
}

float Xeph2D::InputSystem::GetTriggerThreshold()
{
	return Get().m_gamepadHandler->GetTriggerThreshold();
}

void Xeph2D::InputSystem::SetTriggerThreshold(const float threshold)
{
	Get().m_gamepadHandler->SetTriggerThreshold(threshold);
}

bool Xeph2D::InputSystem::GetKeyHold(Key keycode)
{
	return Get().m_keyHandler->GetKeyHold(keycode);
}

bool Xeph2D::InputSystem::GetKeyDown(Key keycode)
{
	return Get().m_keyHandler->GetKeyDown(keycode);
}

bool Xeph2D::InputSystem::GetKeyUp(Key keycode)
{
	return Get().m_keyHandler->GetKeyUp(keycode);
}

bool Xeph2D::InputSystem::GetMouseHold(Mouse::Button btncode)
{
	return Get().m_keyHandler->GetMouseHold(btncode);
}

bool Xeph2D::InputSystem::GetMouseDown(Mouse::Button btncode)
{
	return Get().m_keyHandler->GetMouseDown(btncode);
}

bool Xeph2D::InputSystem::GetMouseUp(Mouse::Button btncode)
{
	return Get().m_keyHandler->GetMouseUp(btncode);
}

float Xeph2D::InputSystem::GetKeyAxisComposite1D(Key negative, Key positive)
{
	return Get().m_keyHandler->GetKeyAxisComposite1D(negative, positive);
}

void Xeph2D::InputSystem::GetKeyAxisComposite2D(float* out_v2, Key negX, Key posX, Key negY, Key posY)
{
	return Get().m_keyHandler->GetKeyAxisComposite2D(out_v2, negX, posX, negY, posY);
}

void Xeph2D::InputSystem::GetMousePos(float* out_v2, bool relativeToWindow)
{
	Get()._GetMousePos(out_v2, relativeToWindow);
}

void Xeph2D::InputSystem::GetMouseDelta(float* out_v2)
{
	out_v2[0] = Get().m_mouseDelta[0];
	out_v2[1] = Get().m_mouseDelta[1];
}

void Xeph2D::InputSystem::SetCaptureMouse(const bool captureMouse)
{
	if (!Get().m_hwnd) return;
	Get().m_captureMouse = captureMouse;
}

bool Xeph2D::InputSystem::GetCaptureMouse()
{
	return Get().m_captureMouse;
}

bool Xeph2D::InputSystem::MouseOverWindow()
{
	if (!Get().m_hwnd) return false;
	float pos[2];
	RECT clientRect;
	GetMousePos(pos, true);
	GetClientRect(Get().m_hwnd, &clientRect);
	return !(pos[0] < 0 || pos[0] >= (float)clientRect.bottom || pos[1] < 0 || pos[1] >= (float)clientRect.right);
}

Xeph2D::InputActionMap* Xeph2D::InputSystem::CreateInputActionMap(std::string name)
{
	InputActionMap* map = Get().m_actionMaps.emplace_back(std::make_unique<InputActionMap>()).get();
	map->name = name;
	return map;
}

InputActionMap* Xeph2D::InputSystem::FindInputActionMap(std::string name)
{
	auto it = std::find_if(Get().m_actionMaps.begin(), Get().m_actionMaps.end(), [=](const std::unique_ptr<InputActionMap>& x) {return x->name == name; });
	return (it == Get().m_actionMaps.end()) ? nullptr : it->get();
}

//================================================================================
//                       IMPLEMENTATION
//================================================================================


void Xeph2D::InputSystem::_Initialize(HWND& hwnd)
{
	m_hwnd = hwnd;
	m_gamepadHandler->Initialize();
}

void Xeph2D::InputSystem::_Update()
{
	POINT newPos;
	GetCursorPos(&newPos);
	if (m_captureMouse)
	{
		if (m_hwnd == nullptr) return; // Cannot capture mouse while window is null
		POINT center = WindowCenter(m_hwnd);

		m_mouseDelta[0] = newPos.x - center.x;
		m_mouseDelta[1] = newPos.y - center.y;
		SetCursorPos(center.x, center.y);
	}
	else
	{
		m_mouseDelta[0] = newPos.x - m_mousePos.x;
		m_mouseDelta[1] = newPos.y - m_mousePos.y;
		m_mousePos = newPos;
	}

	m_gamepadHandler->Update();
	m_keyHandler->Update();
	for (auto& map : m_actionMaps)
		map->Update();
}

void Xeph2D::InputSystem::_GetMousePos(float* out_v2, bool relativeToWindow)
{
	if (m_hwnd && relativeToWindow)
	{
		POINT copy = m_mousePos;
		ScreenToClient(m_hwnd, &copy);
		out_v2[0] = copy.x;
		out_v2[1] = copy.y;
	}
	else
	{
		out_v2[0] = m_mousePos.x;
		out_v2[1] = m_mousePos.y;
	}

}
