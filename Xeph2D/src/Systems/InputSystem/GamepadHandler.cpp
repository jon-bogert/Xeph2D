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

#include "GamepadHandler.h"

#pragma comment (lib, "XInput.lib")

using namespace Xeph2D;

#define BUTTONAXIS_LT 0x1000
#define BUTTONAXIS_RT 0x2000
#define BUTTONAXIS_LS_UP 0x4000
#define BUTTONAXIS_LS_DOWN 0x8000
#define BUTTONAXIS_LS_LEFT 0x0100
#define BUTTONAXIS_LS_RIGHT 0x0200
#define BUTTONAXIS_RS_UP 0x0400
#define BUTTONAXIS_RS_DOWN 0x0800
#define BUTTONAXIS_RS_LEFT 0x0010
#define BUTTONAXIS_RS_RIGHT 0x0020

namespace
{
	const float BYTE_2_FLT = 1.f / 255.f;
	const float STICK_2_FLT = 1.f / 32767.5;
	const float TRI_MAG = 1.f / std::sqrtf(2.f);

	float ApplyDeadzone(float val, float inMin, float inMax)
	{
		bool isNeg = (val < 0.f);
		if (isNeg) val *= -1;
		if (val < inMin) val = inMin;
		float rangeFactor = 1.f / (inMax - inMin);
		float result = (val - inMin) * rangeFactor;
		if (isNeg) result *= -1.f;
		return  result;
	}

	bool GamepadXInput(Gamepad::Button button, WORD& out) //returns true if normal button
	{
		switch (button)
		{
		case Gamepad::Button::A:
			out = XINPUT_GAMEPAD_A;
			return true;
		case Gamepad::Button::B:
			out = XINPUT_GAMEPAD_B;
			return true;
		case Gamepad::Button::X:
			out = XINPUT_GAMEPAD_X;
			return true;
		case Gamepad::Button::Y:
			out = XINPUT_GAMEPAD_Y;
			return true;
		case Gamepad::Button::LB:
			out = XINPUT_GAMEPAD_LEFT_SHOULDER;
			return true;
		case Gamepad::Button::RB:
			out = XINPUT_GAMEPAD_RIGHT_SHOULDER;
			return true;
		case Gamepad::Button::LS_Press:
			out = XINPUT_GAMEPAD_LEFT_THUMB;
			return true;
		case Gamepad::Button::RS_Press:
			out = XINPUT_GAMEPAD_RIGHT_THUMB;
			return true;
		case Gamepad::Button::Start:
			out = XINPUT_GAMEPAD_START;
			return true;
		case Gamepad::Button::Select:
			out = XINPUT_GAMEPAD_BACK;
			return true;
		case Gamepad::Button::DPad_Up:
			out = XINPUT_GAMEPAD_DPAD_UP;
			return true;
		case Gamepad::Button::DPad_Down:
			out = XINPUT_GAMEPAD_DPAD_DOWN;
			return true;
		case Gamepad::Button::DPad_Left:
			out = XINPUT_GAMEPAD_DPAD_LEFT;
			return true;
		case Gamepad::Button::DPad_Right:
			out = XINPUT_GAMEPAD_DPAD_RIGHT;
			return true;
			//===================================================
		case Gamepad::Button::LT:
			out = BUTTONAXIS_LT;
			return false;
		case Gamepad::Button::RT:
			out = BUTTONAXIS_RT;
			return false;
		case Gamepad::Button::LS_Up:
			out = BUTTONAXIS_LS_UP;
			return false;
		case Gamepad::Button::LS_Down:
			out = BUTTONAXIS_LS_DOWN;
			return false;
		case Gamepad::Button::LS_Left:
			out = BUTTONAXIS_LS_LEFT;
			return false;
		case Gamepad::Button::LS_Right:
			out = BUTTONAXIS_LS_RIGHT;
			return false;
		case Gamepad::Button::RS_Up:
			out = BUTTONAXIS_RS_UP;
			return false;
		case Gamepad::Button::RS_Down:
			out = BUTTONAXIS_RS_DOWN;
			return false;
		case Gamepad::Button::RS_Left:
			out = BUTTONAXIS_RS_LEFT;
			return false;
		case Gamepad::Button::RS_Right:
			out = BUTTONAXIS_RS_RIGHT;
			return false;
		default:
			std::cout << "Bad Input" << std::endl;
			return 0;
		}
	}
}

void Xeph2D::GamepadHandler::Initialize()
{
	for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		DWORD result = XInputGetState(i, &m_state);
		if (result == ERROR_SUCCESS)
			m_controllerCount++;
	}
	for (uint8_t i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		m_controllerButtonHold[i] = 0;
		m_controllerButtonDown[i] = 0;
		m_controllerButtonUp[i] = 0;
	}
}

void Xeph2D::GamepadHandler::Update()
{
	uint8_t controllerCount = 0;
	DWORD result;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		ZeroMemory(&m_state, sizeof(XINPUT_STATE));
		result = XInputGetState(i, &m_state);
		if (result == ERROR_SUCCESS)
		{
			controllerCount++;
			WORD buttonState = m_state.Gamepad.wButtons;

			//Get button/up down
			m_controllerButtonDown[i] = ~m_controllerButtonHold[i] & buttonState;
			m_controllerButtonUp[i] = m_controllerButtonHold[i] & ~buttonState;

			//Store current values
			m_controllerButtonHold[i] = buttonState;

			// Update axis
			m_controllerAxisState[i].lx = (static_cast<short>(m_state.Gamepad.sThumbLX) + 0.5f) * STICK_2_FLT;
			m_controllerAxisState[i].ly = (static_cast<short>(m_state.Gamepad.sThumbLY) + 0.5f) * STICK_2_FLT;
			m_controllerAxisState[i].rx = (static_cast<short>(m_state.Gamepad.sThumbRX) + 0.5f) * STICK_2_FLT;
			m_controllerAxisState[i].ry = (static_cast<short>(m_state.Gamepad.sThumbRY) + 0.5f) * STICK_2_FLT;

			m_controllerAxisState[i].lt = static_cast<uint8_t>(m_state.Gamepad.bLeftTrigger) * BYTE_2_FLT;
			m_controllerAxisState[i].rt = static_cast<uint8_t>(m_state.Gamepad.bRightTrigger) * BYTE_2_FLT;

			UpdateAxisState(i);
		}
	}
	if (m_controllerCount != controllerCount)
	{
		m_controllerCountChange = true;
		m_controllerCount = controllerCount;
	}
}

bool Xeph2D::GamepadHandler::IsControllerConnected(const uint8_t num)
{
	return num < m_controllerCount;
}

bool Xeph2D::GamepadHandler::GetGamepadHold(Gamepad::Button button, uint8_t num)
{
	WORD result;
	if (GamepadXInput(button, result))
	{
		return m_controllerButtonHold[num] & result;
	}
	return m_controllerAxisHold[num] & result;
}

bool Xeph2D::GamepadHandler::GetGamepadDown(Gamepad::Button button, uint8_t num)
{
	WORD result;
	if (GamepadXInput(button, result))
	{
		return m_controllerButtonDown[num] & result;
	}
	return m_controllerAxisDown[num] & result;
}

bool Xeph2D::GamepadHandler::GetGamepadUp(Gamepad::Button button, uint8_t num)
{
	WORD result;
	if (GamepadXInput(button, result))
	{
		return m_controllerButtonUp[num] & result;
	}
	return m_controllerAxisUp[num] & result;
}

void Xeph2D::GamepadHandler::GetGamepadAxis(float* out_v2, Gamepad::Axis axis, uint8_t num)
{
	switch (axis)
	{
	case Gamepad::Axis::LS:
		*out_v2 = ApplyDeadzone(m_controllerAxisState[num].lx, m_deadzoneMinimum, m_deadzoneMaximum);
		*(out_v2 + 1) = ApplyDeadzone(m_controllerAxisState[num].ly, m_deadzoneMinimum, m_deadzoneMaximum);
		return;
	case Gamepad::Axis::RS:
		*out_v2 = ApplyDeadzone(m_controllerAxisState[num].rx, m_deadzoneMinimum, m_deadzoneMaximum);
		*(out_v2 + 1) = ApplyDeadzone(m_controllerAxisState[num].ry, m_deadzoneMinimum, m_deadzoneMaximum);
		return;
	case Gamepad::Axis::Trig:
		*out_v2 = m_controllerAxisState[num].lt;
		*(out_v2 + 1) = m_controllerAxisState[num].rt;
		return;
	case Gamepad::Axis::DPad:
		float x = (float)GetGamepadHold(Gamepad::Button::DPad_Right) - (float)GetGamepadHold(Gamepad::Button::DPad_Left);
		float y = (float)GetGamepadHold(Gamepad::Button::DPad_Up) - (float)GetGamepadHold(Gamepad::Button::DPad_Down);
		if (std::fabsf(x) > FLT_EPSILON && std::fabsf(y) > FLT_EPSILON)
		{
			x *= TRI_MAG;
			y *= TRI_MAG;
		}
		*out_v2 = x;
		*(out_v2 + 1) = y;
		return;
	}
}

float Xeph2D::GamepadHandler::GetTriggerThreshold()
{
	return m_triggerThreshold;
}

void Xeph2D::GamepadHandler::SetTriggerThreshold(const float threshold)
{
	m_triggerThreshold = threshold;
}

void Xeph2D::GamepadHandler::UpdateAxisState(uint8_t index)
{
	WORD newState = 0;
	if (m_controllerAxisState[index].lt >= m_triggerThreshold)
		newState |= BUTTONAXIS_LT;
	if (m_controllerAxisState[index].rt >= m_triggerThreshold)
		newState |= BUTTONAXIS_RT;

	if (m_controllerAxisState[index].ly >= m_deadzoneMinimum)
		newState |= BUTTONAXIS_LS_UP;
	if (m_controllerAxisState[index].ly <= -m_deadzoneMinimum)
		newState |= BUTTONAXIS_LS_DOWN;
	if (m_controllerAxisState[index].lx >= m_deadzoneMinimum)
		newState |= BUTTONAXIS_LS_RIGHT;
	if (m_controllerAxisState[index].lx <= -m_deadzoneMinimum)
		newState |= BUTTONAXIS_LS_LEFT;

	if (m_controllerAxisState[index].ry >= m_deadzoneMinimum)
		newState |= BUTTONAXIS_RS_UP;
	if (m_controllerAxisState[index].ry <= -m_deadzoneMinimum)
		newState |= BUTTONAXIS_RS_DOWN;
	if (m_controllerAxisState[index].rx >= m_deadzoneMinimum)
		newState |= BUTTONAXIS_RS_RIGHT;
	if (m_controllerAxisState[index].rx <= -m_deadzoneMinimum)
		newState |= BUTTONAXIS_RS_LEFT;

	//Get button/up down
	m_controllerAxisDown[index] = ~m_controllerAxisHold[index] & newState;
	m_controllerAxisUp[index] = m_controllerAxisHold[index] & ~newState;

	//Store current values
	m_controllerAxisHold[index] = newState;
}
