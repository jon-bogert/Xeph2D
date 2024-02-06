#ifndef __XEPH2D_SCENE_H__
#define __XEPH2D_SCENE_H__

#include "Xeph2D/Ref.h"

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
		
		template <typename Comp>
		Ref<Comp> FindObjectOfType()
		{
			static_assert(std::is_base_of_v<Component, Comp>, "FindObjectOfType Type must be of type Component");
			for (auto& sp_object : m_gameObjects)
			{
				Ref<Comp> result = sp_object->GetComponent<Comp>();
				if (result != nullptr)
					return result;
			}
			return Ref<Comp>();
		}

		template <typename Comp>
		std::vector<Ref<Comp>> FindObjectsOfType()
		{
			static_assert(std::is_base_of_v<Component, Comp>, "FindObjectOfType Type must be of type Component");
			std::vector<Ref<Comp>> result;
			for (auto& sp_object : m_gameObjects)
			{
				Ref<Comp> current = sp_object->GetComponent<Comp>();
				if (current != nullptr)
					result.push_back(current);
			}
			return result;
		}

		Ref<GameObject> CreateObject();
		void DestroyObject(const Ref<GameObject>& object);

	private:
		friend class Runtime;
		void Update();

		friend class SceneManager;
		void Initialize();
		void Shutdown();

		void CheckObjectBuffers();

#ifdef _EDITOR
		friend class Edit::Hierarchy;
#endif //EDITOR
		using GameObjects = std::vector<std::shared_ptr<GameObject>>;
		GameObjects m_gameObjects;
		GameObjects m_addBuffer;
		std::vector<Ref<GameObject>> m_destroyBuffer;

		std::string m_name = "Untitled";
		bool m_initialized = false;
	};
}

#endif //!__XEPH2D_SCENE_H__