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

#ifndef __XEPHINPUT_INPUTSYSTEM_H__
#define __XEPHINPUT_INPUTSYSTEM_H__

#include <Windows.h>
#include <Xinput.h>
#include <iostream>
#include <list>

#include <Xeph2D/Systems/InputSystem/Enums.h>
#include <Xeph2D/Systems/InputSystem/InputActionMap.h>

namespace Xeph2D
{

    class GamepadHandler;
    class KeyHandler;
	class InputSystem final
	{
	public:
        ~InputSystem();

		static void Initialize(HWND& hwnd, std::function<void(InputSystem*)> func);
		static void Update();

        static bool IsControllerConnected(const uint8_t num);
        static bool GetGamepadHold(Gamepad::Button button, uint8_t num = 0);
        static bool GetGamepadDown(Gamepad::Button button, uint8_t num = 0);
        static bool GetGamepadUp(Gamepad::Button button, uint8_t num = 0);
        static void GetGamepadAxis(float* out_v2, Gamepad::Axis axis, uint8_t num = 0);

        static float GetTriggerThreshold();
        static void SetTriggerThreshold(const float threshold);

        static bool GetKeyHold(Key keycode);
        static bool GetKeyDown(Key keycode);
        static bool GetKeyUp(Key keycode);
        static bool GetMouseHold(Mouse::Button btncode);
        static bool GetMouseDown(Mouse::Button btncode);
        static bool GetMouseUp(Mouse::Button btncode);
        static float GetKeyAxisComposite1D(Key negative, Key positive);
        static void GetKeyAxisComposite2D(float* out_v2, Key negX, Key posX, Key negY, Key posY);

        static void GetMousePos(float* out_v2, bool relativeToWindow = true);
        static void GetMouseDelta(float* out_v2);

        static void SetCaptureMouse(const bool captureMouse);
        static bool GetCaptureMouse();
        static bool MouseOverWindow();

        //Creates a new InputActionMap and returns a reference pointer to it
        static InputActionMap* CreateInputActionMap(std::string name);
        //returns reference to an Input Action Map by name
        static InputActionMap* FindInputActionMap(std::string name);

	private:
		// Public impl
		void _Initialize(HWND& hwnd);
		void _Update();

        void _GetMousePos(float* out_v2, bool relativeToWindow);

        InputSystem();
        static InputSystem& Get();

        HWND m_hwnd = nullptr;

        GamepadHandler* m_gamepadHandler = nullptr;
        KeyHandler* m_keyHandler = nullptr;

        POINT m_mousePos;
        float m_mouseDelta[2] = { 0.f, 0.f };
        bool m_captureMouse = false;

        std::list<std::unique_ptr<InputActionMap>> m_actionMaps;
	};
}

#endif // !__XEPHINPUT_INPUTSYSTEM_H__