#ifdef _EDITOR
#include "Xeph2D/Editor/EditorWindows/Inspector.h"
#include "Xeph2D/Editor/Editor.h"

#include <variant>

void Xeph2D::Edit::Inspector::Initialize()
{
	name = "Inspector";
}

void Xeph2D::Edit::Inspector::OnGUI()
{
	if (m_activeIndex < 0)
		return;

	EditorGameObject& activeObject = Editor::Get().m_sceneData.gameObjects[m_activeIndex];
	char buffer[MAX_PATH];

	//ID
	std::string line;
	strcpy(buffer, Utility::ToHex32String(activeObject.instID).substr(2).c_str());
	ImGui::InputText("ID", buffer, ImGuiInputTextFlags_ReadOnly);

	//Name
	ShowData(activeObject.name, 0);

	//Active
	ShowData(activeObject.isActive, 0);

	//Transform;
	ImGui::NewLine();
	ShowData(activeObject.transform, 0);
	ImGui::NewLine();

	for (EditorComponent& component : activeObject.components)
	{
		std::string compName = Editor::Get().m_componentNames[component.typeID];
		if (ImGui::CollapsingHeader(compName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			ShowData(component.enabled, component.typeID);
			for (Field& field : component.fields)
			{
				ShowData(field, component.typeID);
			}
		}
		ImGui::NewLine();
	}
}

void Xeph2D::Edit::Inspector::SetActiveIndex(int index)
{
	m_activeIndex = index;
	if (index >= 0)
	{
		Editor::Get().GetTransformGizmo()->SetCurrentObject(&Editor::Get().m_sceneData.gameObjects[m_activeIndex]);
		return;
	}
	Editor::Get().GetTransformGizmo()->SetCurrentObject(nullptr);
}

void Xeph2D::Edit::Inspector::ShowData(Field& field, uint32_t compID, bool showName, SerializableType type)
{
	std::string display;
	if (type == SerializableType::Undefined)
	{
		type = field.type;
	}
	if (showName)
		display = Var2DisplayName(field.name) + "##" + field.name + Utility::ToHex32String(compID);
	else
		display = "##" + field.name + Utility::ToHex32String(compID);

	char charBuff{};
	char strBuff[MAX_PATH];
	Transform* transform = nullptr;
	float rotBuff{};

	switch (type)
	{
	case SerializableType::Int:
		if (ImGui::DragInt(display.c_str(), static_cast<int*>(field.ptr)))
			Editor::SetIsSaved(false);
		break;
	case SerializableType::Float:
		if (ImGui::DragFloat(display.c_str(), static_cast<float*>(field.ptr), 0.01f));
			Editor::SetIsSaved(false);
		break;
	case SerializableType::Bool:
		if (ImGui::Checkbox(display.c_str(), static_cast<bool*>(field.ptr)))
			Editor::SetIsSaved(false);
		break;
	case SerializableType::Char:
		charBuff = *(char*)field.ptr;
		if (ImGui::InputText(display.c_str(), &charBuff, 1))
		{
			*(char*)field.ptr = charBuff;
			Editor::SetIsSaved(false);
		}
		break;
	case SerializableType::String:
		strcpy(strBuff, field.As<std::string>().c_str());
		if (ImGui::InputText(display.c_str(), strBuff, MAX_PATH - 1))
		{
			field.As<std::string>() = strBuff;
			Editor::SetIsSaved(false);
		}
		break;
	case SerializableType::Vector2:
		if (ImGui::DragFloat2(display.c_str(), &(field.As<Vector2>().x), 0.01f))
		{
			Editor::SetIsSaved(false);
		}
		break;
	case SerializableType::Color:
		if (ImGui::ColorEdit4(display.c_str(), &(field.As<Color>().r), 0.01f))
		{
			Editor::SetIsSaved(false);
		}
		break;
	case SerializableType::Transform:
		transform = static_cast<Transform*>(field.ptr);
		if (showName)
			ImGui::Text(Var2DisplayName(field.name).c_str());
		//Position
		if (ImGui::DragFloat2(("Position##P" + display).c_str(), &transform->position.x, 0.01f))
		{
			Editor::SetIsSaved(false);
		}
		//Position
		rotBuff = transform->rotation.GetDeg();
		if (ImGui::DragFloat(("Rotation##R" + display).c_str(), &rotBuff, 0.01f))
		{
			transform->rotation.SetDeg(rotBuff);
			Editor::SetIsSaved(false);
		}
		//Scale
		if (ImGui::DragFloat2(("Scale##S" + display).c_str(), &transform->scale.x, 0.01f))
		{
			Editor::SetIsSaved(false);
		}
		break;
	}
}

#define __CAP_OFFSET 32

std::string Xeph2D::Edit::Inspector::Var2DisplayName(std::string varName)
{
	bool spaceLast = false;
	if (varName.size() <= 2)
		return varName;

	if (varName.substr(0, 2) == "m_")
		varName = varName.substr(2);
	else if (varName[0] == 'm' && varName[1] <= 'Z')
		varName = varName.substr(1);

	while (varName[0] == '_')
		varName = varName.substr(1);

	if (varName[0] >= 'a')
		varName[0] -= __CAP_OFFSET;
	for (size_t i = 1; i < varName.size();)
	{
		if (varName[i] == '_' && varName.size() >= i + 2)
		{
			if (varName[i + 1] == '_')
			{
				varName.erase(i, 1);
				continue;
			}
			varName[i] = ' ';
			if (varName[i + 1] > 'Z')
			{
				varName[i + 1] -= __CAP_OFFSET;
			}
			i += 2;
			spaceLast = true;
			continue;
		}
		else if (varName[i] == '_')
		{
			varName.pop_back();
			continue;
		}

		if (varName[i] <= 'Z' && !spaceLast)
		{
			spaceLast = 0;
			varName.insert(i, 1, ' ');
			i += 2;
			continue;
		}
		else if (varName[i] > 'Z' && spaceLast)
			spaceLast = false;

		++i;
	}

	return varName;
}

#endif //_EDITOR