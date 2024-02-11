#ifndef __X2D_EDITOR_EDITORWINDOWS_PROJECTSETTINGS_H__
#define __X2D_EDITOR_EDITORWINDOWS_PROJECTSETTINGS_H__

#ifdef _EDITOR

#include "Xeph2D/Editor/EditorWindow.h"

#include <yaml-cpp/yaml.h>

#include <vector>
#include <string>

namespace Xeph2D::Edit
{
	class ProjectSettings final : public EditorWindow
	{
	public:
		void Initialize() override;
		void OnGUI() override;

	private:
		void ApplicationPage();
		void DisplayPage();
		void BuildPage();

		void AddBuildScene();
		void RemoveBuildScene();
		void BuildSceneUp();
		void BuildSceneDown();

		void SaveFiles();

		enum class Page // NOTE: Maintain Alpha-older
		{
			Application,
			Build,
			Display,
		};
		std::vector<std::string> m_pageNames
		{
			"Application",
			"Build",
			"Display",
		};

		int m_indexSelection = -1;

		YAML::Node m_buildData;
		YAML::Node m_displayData;
		const std::string m_buildFile = "BuildInfo.yaml";
		const std::string m_displayFile = "WindowProperties.yaml";

		int m_buildIndexSelection = -1;
	};
}

#endif // _EDITOR
#endif //!__X2D_EDITOR_EDITORWINDOWS_PROJECTSETTINGS_H__
