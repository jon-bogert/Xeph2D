#ifdef _EDITOR
#include "Xeph2D/Editor/EditorWindows/Inspector.h"
#include "Xeph2D/Editor/Editor.h"

#include <variant>
#include <regex>

void Xeph2D::Edit::Inspector::Initialize()
{
	name = "Inspector";
}

void Xeph2D::Edit::Inspector::OnGUI()
{
	if (m_activeIndex < 0)
		return;

	if (m_showEdit)
	{
		DrawEdit();
		return;
	}

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
		std::string compName = Editor::Get().GetComponentManager()->GetComponentName(component.typeID);
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
	ImGui::Separator();
	ImGui::NewLine();
	if (ImGui::Button("Edit Components##Insp"))
	{
		m_showEdit = true;
		m_editSelection = -1;
	}
}

void Xeph2D::Edit::Inspector::SetActiveIndex(int index)
{
	m_activeIndex = index;
	m_showEdit = false;
	m_showAdd = false;
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

void Xeph2D::Edit::Inspector::DrawEdit()
{
	std::vector<std::string> itemNames;
	ComponentManager& scriptManager = *Editor::Get().GetComponentManager();
	for (EditorComponent& activeComp : Editor::Get().m_sceneData.gameObjects[m_activeIndex].components)
	{
		itemNames.push_back(scriptManager.GetComponentName(activeComp.typeID));
	}
	if (ImGui::ListBox("##HItems", &m_editSelection, Utility::CStrVect(itemNames).data(), itemNames.size()))
	{

	}
	if (ImGui::Button("+##Insp"))
	{
		m_showAdd = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("-##Insp"))
	{
		if (m_editSelection < 0)
			return;

		Editor::RemoveComponent(m_activeIndex, m_editSelection);
		m_editSelection = -1;
	}
	ImGui::SameLine();
	if (ImGui::Button("^##Insp"))
	{
		if (Editor::ComponentOrderUp(m_activeIndex, m_editSelection))
		{
			--m_editSelection;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("v##Insp"))
	{
		if (Editor::ComponentOrderDown(m_activeIndex, m_editSelection))
		{
			++m_editSelection;
		}
	}
	if (m_showAdd)
	{
		ImVec2 nextPos = (Vector2)ImGui::GetCursorPos() + ImGui::GetWindowPos();
		ImGui::SetNextWindowPos(nextPos);
		ImGui::Begin("Add Component##Insp", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
		if (InputSystem::GetMouseDown(Mouse::Button::Left) && !ImGui::IsWindowHovered())
			m_showAdd = false;
	
		std::vector<std::string> compNames = Editor::Get().GetComponentManager()->GetAllNames();
		ImGui::InputText("##SearchAddComp", m_editSearchBuff, MAX_PATH - 1);
		for (auto& comp : compNames)
		{
			if (!std::regex_search(comp, std::regex(m_editSearchBuff)))
				continue;
			if (ImGui::MenuItem((comp + "##AddComp").c_str()))
			{
				uint32_t id = Editor::Get().GetComponentManager()->GetID(comp);
				Editor::AddComponent(m_activeIndex, id);
				Editor::SetIsSaved(false);
				m_showAdd = false;
				break;
			}
		}
		ImGui::End();
	}

	ImGui::NewLine();
	ImGui::Separator();
	ImGui::NewLine();
	if (ImGui::Button("Show Components##Insp"))
	{
		m_showEdit = false;
	}
}


#endif //_EDITOR