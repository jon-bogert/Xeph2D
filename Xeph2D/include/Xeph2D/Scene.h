#ifndef __XEPH2D_SCENE_H__
#define __XEPH2D_SCENE_H__

#include <vector>
#include <string>
#include <memory>

namespace Xeph2D
{
#ifdef _EDITOR
	namespace Edit
	{
		class Hierarchy;
	}
#endif //_EDITOR
	class GameObject;

	class Scene final
	{
	public:
		std::string GetName() const { return m_name; }

	private:
		friend class Runtime;
		void Update();

		friend class SceneManager;
		void Initialize();
		void Shutdown();

#ifdef _EDITOR
		friend class Edit::Hierarchy;
#endif //EDITOR
		using GameObjects = std::vector<std::shared_ptr<GameObject>>;
		GameObjects m_gameObjects;

		std::string m_name = "Untitled";
	};
}

#endif //!__XEPH2D_SCENE_H__