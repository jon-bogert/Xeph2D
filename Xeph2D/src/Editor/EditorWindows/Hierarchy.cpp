#ifdef _EDITOR
#include "Xeph2D/Editor/EditorWindows/Hierarchy.h"

#include "Xeph2D/Editor/Editor.h"
#include "Xeph2D/Systems/SceneManager.h"
#include "Xeph2D/Scene.h"

void Xeph2D::Edit::Hierarchy::Initialize()
{
	name = "Hierarchy";
}

void Xeph2D::Edit::Hierarchy::OnGUI()
{
	std::string sceneName = SceneManager::ActiveScene().GetName();
	if (!Editor::IsSaved())
		sceneName += " - (not saved)";

	ImGui::Text(sceneName.c_str());
	ConstructNameList();
	if (ImGui::ListBox("##HItems", &m_selectionIndex, Utility::CStrVect(m_objectNames).data(), m_objectNames.size()))
	{
		Editor::Get().GetInspectorWindow()->SetActiveIndex(m_selectionIndex);
	}
	if (ImGui::Button("+##AddObject"))
	{
		Editor::AddObject();
		Editor::Get().GetInspectorWindow()->SetActiveIndex(m_selectionIndex);
	}
	ImGui::SameLine();
	if (ImGui::Button("-##RemObject"))
	{
		if (m_selectionIndex < 0)
			return;
		Editor::RemoveObject(m_selectionIndex);
		m_selectionIndex = -1;
		Editor::Get().GetInspectorWindow()->SetActiveIndex(m_selectionIndex);
	}
	ImGui::SameLine();
	if (ImGui::Button("^##UpObject"))
	{
		if (Editor::ObjectOrderUp(m_selectionIndex))
		{
			Editor::Get().GetInspectorWindow()->SetActiveIndex(--m_selectionIndex);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("v##DownObject"))
	{
		if (Editor::ObjectOrderDown(m_selectionIndex))
		{
			Editor::Get().GetInspectorWindow()->SetActiveIndex(++m_selectionIndex);
		}
	}
}

void Xeph2D::Edit::Hierarchy::ConstructNameList()
{
	m_objectNames.clear();
	for (const auto& sp_obj : SceneManager::ActiveScene().m_gameObjects)
	{
		m_objectNames.push_back(sp_obj->GetName());
	}
}

#endif //_EDITOR