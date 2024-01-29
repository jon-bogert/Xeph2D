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

#include "Xeph2D/Systems/InputSystem/InputActionMap.h"
#include "Xeph2D/Systems/InputSystem/InputSystem.h"

Xeph2D::InputAction::InputAction(std::string name, Type type, ButtonEvent buttonEvent)
	: name(name), m_type(type), m_buttonEvent(buttonEvent)
{
	switch (type)
	{
	case Type::Button:
		m_data = static_cast<void*>(new bool);
		break;
	case Type::Axis1D:
		m_data = static_cast<void*>(new float);
		break;
	case Type::Axis2D:
		m_data = static_cast<void*>(new float[2]);
	}
}

Xeph2D::InputAction::~InputAction()
{
	switch (m_type)
	{
	case Type::Button:
		delete static_cast<bool*>(m_data);
		break;
	case Type::Axis1D:
		delete static_cast<float*>(m_data);
		break;
	case Type::Axis2D:
		delete[] static_cast<float*>(m_data);
	}
}

Xeph2D::InputAction::Type Xeph2D::InputAction::GetType() const
{
	return m_type;
}

void Xeph2D::InputAction::AddButton(Gamepad::Button button, uint8_t player)
{
	if (m_type != Type::Button) return; // return if not of type
	auto it = m_map->m_buttonActions.find({ button, player }); // try to find existing button
	if (it == m_map->m_buttonActions.end()) // was not found, create one
	{
		m_map->m_buttonActions.insert({ { button, player }, {} });
		it = m_map->m_buttonActions.find({ button, player });
	}
	//return if this InputAction is already assigned to this button
	if (std::find(it->second.second.begin(), it->second.second.end(), this) != it->second.second.end()) return;

	//Add InputAction ptr to this button list;
	it->second.second.push_back(this);
}

void Xeph2D::InputAction::AddButton(Key key)
{
	if (m_type != Type::Button) return; // return if not of type
	auto it = m_map->m_keyActions.find(key); // try to find existing button
	if (it == m_map->m_keyActions.end()) // was not found, create one
	{
		m_map->m_keyActions.insert({ key, {} });
		it = m_map->m_keyActions.find(key);
	}
	//return if this InputAction is already assigned to this button
	if (std::find(it->second.second.begin(), it->second.second.end(), this) != it->second.second.end()) return;

	//Add InputAction ptr to this button list;
	it->second.second.push_back(this);
}

void Xeph2D::InputAction::AddButton(Mouse::Button button)
{
	if (m_type != Type::Button) return; // return if not of type
	auto it = m_map->m_mouseActions.find(button); // try to find existing button
	if (it == m_map->m_mouseActions.end()) // was not found, create one
	{
		m_map->m_mouseActions.insert({ button, {} });
		it = m_map->m_mouseActions.find(button);
	}
	//return if this InputAction is already assigned to this button
	if (std::find(it->second.second.begin(), it->second.second.end(), this) != it->second.second.end()) return;

	//Add InputAction ptr to this button list;
	it->second.second.push_back(this);
}

void Xeph2D::InputAction::Add1DAxis(Gamepad::Axis axis, uint8_t component, uint8_t player)
{
	if (m_type != Type::Axis1D) return;
	auto it = m_map->m_1DActions.find({ { axis, player }, component }); // try to find existing input
	if (it == m_map->m_1DActions.end()) // was not found, create one
	{
		m_map->m_1DActions.insert({ {{ axis, player }, component}, {} });
		it = m_map->m_1DActions.find({ { axis, player }, component });
	}
	//return if this InputAction is already assigned to this input
	if (std::find(it->second.second.begin(), it->second.second.end(), this) != it->second.second.end()) return;

	//Add InputAction ptr to this button list;
	it->second.second.push_back(this);
}

void Xeph2D::InputAction::Add1DAxis(Key neg, Key pos)
{
	if (m_type != Type::Axis1D) return;
	auto it = m_map->m_1DCompActions.find({pos, neg}); // try to find existing input
	if (it == m_map->m_1DCompActions.end()) // was not found, create one
	{
		m_map->m_1DCompActions.insert({ {pos, neg}, {} });
		it = m_map->m_1DCompActions.find({ pos, neg });
	}
	//return if this InputAction is already assigned to this input
	if (std::find(it->second.second.begin(), it->second.second.end(), this) != it->second.second.end()) return;

	//Add InputAction ptr to this button list;
	it->second.second.push_back(this);
}

void Xeph2D::InputAction::Add2DAxis(Gamepad::Axis axis, uint8_t player)
{
	if (m_type != Type::Axis2D) return;
	auto it = m_map->m_2DActions.find({ axis, player }); // try to find existing button
	if (it == m_map->m_2DActions.end()) // was not found, create one
	{
		m_map->m_2DActions.insert({ { axis, player }, {} });
		it = m_map->m_2DActions.find({ axis, player });
	}
	//return if this InputAction is already assigned to this button
	if (std::find(it->second.second.begin(), it->second.second.end(), this) != it->second.second.end()) return;

	//Add InputAction ptr to this button list;
	it->second.second.push_back(this);
}

void Xeph2D::InputAction::Add2DAxis(Key negX, Key posX, Key negY, Key posY)
{
	if (m_type != Type::Axis2D) return;
	auto it = m_map->m_2DCompActions.find({ {negX, posX}, {negY, posY} }); // try to find existing button
	if (it == m_map->m_2DCompActions.end()) // was not found, create one
	{
		m_map->m_2DCompActions.insert({ {{negX, posX}, {negY, posY}}, {} });
		it = m_map->m_2DCompActions.find({ {negX, posX}, {negY, posY} });
	}
	//return if this InputAction is already assigned to this button
	if (std::find(it->second.second.begin(), it->second.second.end(), this) != it->second.second.end()) return;

	//Add InputAction ptr to this button list;
	it->second.second.push_back(this);
}

void Xeph2D::InputAction::ReadValue(void* out)
{
	switch (m_type)
	{
	case Type::Button:
		*static_cast<bool*>(out) = *static_cast<bool*>(m_data);
		return;
	case Type::Axis1D:
		*static_cast<float*>(out) = *static_cast<float*>(m_data);
		return;
	case Type::Axis2D:
		*static_cast<float*>(out) = *static_cast<float*>(m_data);
		*(static_cast<float*>(out) + 1) = *(static_cast<float*>(m_data) + 1);
		return;
	default:
		return;
	}
}

void Xeph2D::InputAction::RaiseEvent()
{
	performed.Invoke(this);
}

Xeph2D::InputAction* Xeph2D::InputActionMap::CreateAction(std::string name, InputAction::Type type, InputAction::ButtonEvent buttonEvent)
{
	 m_inputActions.emplace_back(std::make_unique<InputAction>(name, type, buttonEvent));
	 InputAction* ptr = m_inputActions.back().get();
	 ptr->m_map = this;
	 return ptr;
}

Xeph2D::InputAction* Xeph2D::InputActionMap::FindInputAction(std::string name)
{
	auto it = std::find_if(m_inputActions.begin(), m_inputActions.end(), [=](const std::unique_ptr<InputAction>& x) {return x->name == name; });
	return (it == m_inputActions.end()) ? nullptr : it->get();
}

void Xeph2D::InputActionMap::Update()
{
	for (auto& action : m_inputActions) action->m_triggered = false;

	{ // BUTTONS       [ Axis | playernum ]     [ oldVal | vector<pointers> ]
		bool both;
		bool down;
		bool up;
		bool hold;
		for (auto& button : m_buttonActions) // Gamepad Buttons
		{
			hold = InputSystem::GetGamepadHold(button.first.first, button.first.second);
			down = InputSystem::GetGamepadDown(button.first.first, button.first.second);
			up = InputSystem::GetGamepadUp(button.first.first, button.first.second);
			both = down || up;
			for (auto& action : button.second.second)
			{
				if (button.second.first != hold)
				{
					button.second.first = hold;
					if (action->m_triggered) continue;
					action->m_triggered = true;
					*static_cast<bool*>(action->m_data) = hold;
					if (action->m_buttonEvent == InputAction::ButtonEvent::Down && down)
					{
						action->RaiseEvent();
					}
					else if (action->m_buttonEvent == InputAction::ButtonEvent::Up && up)
					{
						action->RaiseEvent();
					}
					else if (action->m_buttonEvent == InputAction::ButtonEvent::DownUp && both)
					{
						action->RaiseEvent();
					}
				}
			}
		}
		for (auto& key : m_keyActions) // Key Buttons
		{
			hold = InputSystem::GetKeyHold(key.first);
			down = InputSystem::GetKeyDown(key.first);
			up = InputSystem::GetKeyUp(key.first);
			both = down || up;
			for (auto& action : key.second.second)
			{
				if (key.second.first != hold)
				{
					key.second.first = hold;
					if (action->m_triggered) continue;
					action->m_triggered = true;
					*static_cast<bool*>(action->m_data) = hold;
					if (action->m_buttonEvent == InputAction::ButtonEvent::Down && down)
					{
						action->RaiseEvent();
					}
					else if (action->m_buttonEvent == InputAction::ButtonEvent::Up && up)
					{
						action->RaiseEvent();
					}
					else if (action->m_buttonEvent == InputAction::ButtonEvent::DownUp && both)
					{
						action->RaiseEvent();
					}
				}
			}
		}
		for (auto& button : m_mouseActions) // Mouse Buttons
		{
			hold = InputSystem::GetMouseHold(button.first);
			down = InputSystem::GetMouseDown(button.first);
			up = InputSystem::GetMouseUp(button.first);
			both = down || up;
			for (auto& action : button.second.second)
			{
				if (button.second.first != hold)
				{
					button.second.first = hold;
					if (action->m_triggered) continue;
					action->m_triggered = true;
					*static_cast<bool*>(action->m_data) = hold;
					if (action->m_buttonEvent == InputAction::ButtonEvent::Down && down)
					{
						action->RaiseEvent();
					}
					else if (action->m_buttonEvent == InputAction::ButtonEvent::Up && up)
					{
						action->RaiseEvent();
					}
					else if (action->m_buttonEvent == InputAction::ButtonEvent::DownUp && both)
					{
						action->RaiseEvent();
					}
				}
			}
		}
	}
	{ // FLOAT       [ [Axis | playernum] | Dimension ]     [ oldVal | vector<pointers> ]
		float oldVal = 0.f;
		float newVal = 0.f;
		float v2[2] = { 0.f, 0.f };
		for (auto& axis : m_1DActions) // Gamepad Axis
		{
			oldVal = axis.second.first;
			InputSystem::GetGamepadAxis(&v2[0], axis.first.first.first, axis.first.first.second);
			newVal = v2[axis.first.second];
			for (auto& action : axis.second.second)
			{
				if (oldVal != newVal)
				{
					axis.second.first = oldVal;
					if (action->m_triggered) continue;
					action->m_triggered = true;
					*static_cast<float*>(action->m_data) = newVal;
					action->RaiseEvent();
				}
			}
		}
		for (auto& axis : m_1DCompActions) // Key Composite Axis
		{
			oldVal = axis.second.first;
			newVal = InputSystem::GetKeyAxisComposite1D(axis.first.first, axis.first.second);
			for (auto& action : axis.second.second)
			{
				if (oldVal != newVal)
				{
					axis.second.first = oldVal;
					if (action->m_triggered) continue;
					action->m_triggered = true;
					*static_cast<float*>(action->m_data) = newVal;
					action->RaiseEvent();
				}
			}
		}
	}
	{ // VEC2       [ Axis | playernum ]     [ oldVal | vector<pointers> ]
		float oldVal[2] = { 0.f, 0.f };
		float newVal[2] = { 0.f, 0.f };
		for (auto& axis : m_2DActions) // Gamepad Axis
		{
			oldVal[0] = axis.second.first[0];
			oldVal[1] = axis.second.first[1];
			InputSystem::GetGamepadAxis(&newVal[0], axis.first.first, axis.first.second);
			for (auto& action : axis.second.second)
			{
				if (oldVal[0] != newVal[0] || oldVal[1] != newVal[1])
				{
					axis.second.first[0] = newVal[0];
					axis.second.first[1] = newVal[1];
					if (action->m_triggered) continue;
					action->m_triggered = true;
					*static_cast<float*>(action->m_data) = newVal[0];
					*(static_cast<float*>(action->m_data) + 1) = newVal[1];
					action->RaiseEvent();
				}
			}
		}
		for (auto& axis : m_2DCompActions) // Key Composite Axis
		{
			oldVal[0] = axis.second.first[0];
			oldVal[1] = axis.second.first[1];
			InputSystem::GetKeyAxisComposite2D(&newVal[0],
				axis.first.first.first,
				axis.first.first.second,
				axis.first.second.first,
				axis.first.second.second);
			for (auto& action : axis.second.second)
			{
				if (oldVal[0] != newVal[0] || oldVal[1] != newVal[1])
				{
					axis.second.first[0] = newVal[0];
					axis.second.first[1] = newVal[1];
					if (action->m_triggered) continue;
					action->m_triggered = true;
					*static_cast<float*>(action->m_data) = newVal[0];
					*(static_cast<float*>(action->m_data) + 1) = newVal[1];
					action->RaiseEvent();
				}
			}
		}
	}
}
