#ifdef _EDITOR
#include "Xeph2D/Editor/EditorWindows/Hierarchy.h"

#include "Xeph2D/Systems/SceneManager.h"
#include "Xeph2D/Scene.h"

void Xeph2D::Edit::Hierarchy::Initialize()
{
	name = "Hierarchy";
	ConstructNameList();
}

void Xeph2D::Edit::Hierarchy::OnGUI()
{
	std::string sceneName = SceneManager::ActiveScene().GetName();
	//Add 'not saved'

	ImGui::Text(sceneName.c_str());
	if (ImGui::ListBox("##HItems", &m_selectionIndex, m_objectNamesCStr.data(), m_objectNames.size()))
	{
		//Editor::GetInspectorWindow()->SetGameObject(objects[m_selectionIndex]);
	}
}

void Xeph2D::Edit::Hierarchy::ConstructNameList()
{
	for (const auto& sp_obj : SceneManager::ActiveScene().m_gameObjects)
	{
		m_objectNames.push_back(sp_obj->GetName());
	}
	m_objectNamesCStr = Utility::CStrVect(m_objectNames);
}

#endif //_EDITOR