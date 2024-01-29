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

#ifndef __XEPHINPUT_GAMEPADHANDLER_H__
#define __XEPHINPUT_GAMEPADHANDLER_H__

#include "Xeph2D/Systems/InputSystem/InputSystem.h"
namespace Xeph2D
{
	class GamepadHandler
	{
		friend class InputSystem;

		struct Axis
		{
			float lx = 0.f;
			float ly = 0.f;
			float rx = 0.f;
			float ry = 0.f;
			float lt = 0.f;
			float rt = 0.f;
		};

		XINPUT_STATE m_state{};

		bool m_controllerActive = true;
		uint8_t  m_controllerCount = 0;
		bool m_controllerCountChange = false;

		WORD m_controllerButtonHold[XUSER_MAX_COUNT];
		WORD m_controllerButtonDown[XUSER_MAX_COUNT];
		WORD m_controllerButtonUp[XUSER_MAX_COUNT];
		WORD m_controllerAxisHold[XUSER_MAX_COUNT];
		WORD m_controllerAxisDown[XUSER_MAX_COUNT];
		WORD m_controllerAxisUp[XUSER_MAX_COUNT];
		Axis m_controllerAxisState[XUSER_MAX_COUNT];

		float m_triggerThreshold = 0.1f;
		float m_deadzoneMinimum = 0.3f;
		float m_deadzoneMaximum = 1.f;

		void Initialize();
		void Update();

		bool IsControllerConnected(const uint8_t num);
		bool GetGamepadHold(Gamepad::Button button, uint8_t num = 0);
		bool GetGamepadDown(Gamepad::Button button, uint8_t num = 0);
		bool GetGamepadUp(Gamepad::Button button, uint8_t num = 0);
		void GetGamepadAxis(float* out_v2, Gamepad::Axis axis, uint8_t num = 0);

		float GetTriggerThreshold();
		void SetTriggerThreshold(const float threshold);

		void UpdateAxisState(uint8_t index);
	};
}

#endif //!__XEPHINPUT_GAMEPADHANDLER_H__