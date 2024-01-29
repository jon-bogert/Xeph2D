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

#ifndef __XEPHINPUT_INPUTACTIONMAP_H__
#define __XEPHINPUT_INPUTACTIONMAP_H__

#include <functional>
#include <memory>
#include <map>
#include <vector>
#include <string>

#include "Xeph2D/Systems/InputSystem/Enums.h"


#define XEInputActionCallback(function) static_cast<void*>(this), std::bind(&function, this, std::placeholders::_1)
#define XEInputActionCallbackPtr(function, ptr) static_cast<void*>(ptr), std::bind(&function, ptr, std::placeholders::_1)

namespace Xeph2D
{
    class InputAction;
    class InputActionMap;

    class InputActionEvent
    {
        friend class InputAction;

    public:
        void Subscribe(void* object, std::function<void(InputAction*)> function)
        {
            m_events.push_back({ object, function });
        }

        void UnsubscribeAll(void* object)
        {
            m_events.remove_if([=](const EventEntry& x) {return object == x.object; });
        }

    private:
        struct EventEntry
        {
            void* object;
            std::function<void(InputAction*)> function;
        };

        void Invoke(InputAction* agent) {
            for (const auto& event : m_events)
            {
                event.function(agent);
            }
        }

        std::list<EventEntry> m_events;
    };

    class InputAction
    {
        friend class InputActionMap;
    public:
        enum class Type { Button, Axis1D, Axis2D };
        enum class ButtonEvent { Down, Up, DownUp };

        std::string name = "";
        InputActionEvent performed; // subscribed events will be triggered when action is performed

        InputAction(std::string name = "", Type type = Type::Button, ButtonEvent buttonEvent = ButtonEvent::Down);
        virtual ~InputAction();

        Type GetType() const; 

        //Add bindings based on the Input Type
        void AddButton(Gamepad::Button button, uint8_t player = 0);
        void AddButton(Key key);
        void AddButton(Mouse::Button button);
        void Add1DAxis(Gamepad::Axis axis, uint8_t component, uint8_t player = 0);
        void Add1DAxis(Key neg, Key pos);
        void Add2DAxis(Gamepad::Axis axis, uint8_t player = 0);
        void Add2DAxis(Key negX, Key posX, Key negY, Key posY);

        //TODO Remove Functions

        //populates the provided pointer with appropriate data based on Input Type
        void ReadValue(void* out);

    private:
        void RaiseEvent();

        Type m_type = Type::Button;
        ButtonEvent m_buttonEvent = ButtonEvent::Down; // Whether ".performed" will be called on button press, release, or both (Type::Button only)

        InputActionMap* m_map = nullptr; // reference to parent map
        void* m_data;
        bool m_triggered = false; // flag as to whether ".performed" event has already been invoked by another binding this frame;
        
    };

    class InputActionMap
    {
        friend class InputAction;
    public:
        std::string name = "";
        //Adds an InputAction and returns a reference to it as a pointer
        InputAction* CreateAction(std::string name, InputAction::Type type = InputAction::Type::Button, InputAction::ButtonEvent buttonEvent = InputAction::ButtonEvent::Down);
        InputAction* FindInputAction(std::string name);
        void Update();

    private:
        //List of all Input Actions
        std::list<std::unique_ptr<InputAction>> m_inputActions;

        //References to each input action based on their input types
        std::map<std::pair<Gamepad::Button, uint8_t>, std::pair<bool, std::vector<InputAction*>>> m_buttonActions;
        std::map<Key, std::pair<bool, std::vector<InputAction*>>> m_keyActions;
        std::map<Mouse::Button, std::pair<bool, std::vector<InputAction*>>> m_mouseActions;
        std::map<std::pair<std::pair<Gamepad::Axis, uint8_t>, uint8_t>, std::pair<float, std::vector<InputAction*>>> m_1DActions;
        std::map<std::pair<Key, Key>, std::pair<float, std::vector<InputAction*>>> m_1DCompActions;
        std::map<std::pair<Gamepad::Axis, uint8_t>, std::pair<float[2], std::vector<InputAction*>>> m_2DActions;
        std::map<std::pair<std::pair<Key, Key>, std::pair<Key, Key>>, std::pair<float[2], std::vector<InputAction*>>> m_2DCompActions;
    };
}

#endif // !__XEPHINPUT_INPUTACTIONMAP_H__