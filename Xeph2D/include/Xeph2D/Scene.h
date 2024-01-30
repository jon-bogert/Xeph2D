#ifndef __XEPH2D_SCENE_H__
#define __XEPH2D_SCENE_H__

#include <vector>
#include <memory>

namespace Xeph2D
{
	class GameObject;

	class Scene final
	{
	private:
		friend class Runtime;
		void Update();

		friend class SceneManager;
		void Initialize();
		void Shutdown();

		using GameObjects = std::vector<std::shared_ptr<GameObject>>;
		GameObjects m_gameObjects;
	};
}

#endif //!__XEPH2D_SCENE_H__