#ifndef __XEPH2D_GAMEOBJECT_H__
#define __XEPH2D_GAMEOBJECT_H__

#include "Xeph2D/Structs.h"
#include "Xeph2D/Systems/SceneManager.h"
#include "Xeph2D/Scene.h"

#include <memory>
#include <string>
#include <vector>

namespace Xeph2D
{
#ifdef _EDITOR
	namespace Edit
	{
		class Editor;
	}
#endif //_EDITOR
	template <typename ComponentType>
	class Ref;

	class Component;
	class GameObject final
	{
#ifdef _EDITOR
		friend class Edit::Editor;
#endif //_EDITOR
		friend class WindowManager;
	public:
		uint32_t InstID() { return m_instID; }
		std::string GetName() { return m_name; }

		bool IsActive() const { return m_isActive; };
		void SetIsActive(const bool isActive);

		//Global Transforms
		//Vector2 Position() const;
		//Rotator Rotation() const;
		//Vector2 Scale() const;
		//void SetPosition(const Vector3 position);
		//void SetRotation(const Rotator rotation);
		//void SetRotation(const float rotation);

		Vector2& LocalPosition() { return m_transform.position; }
		Rotator& LocalRotation() { return m_transform.rotation; }
		Vector2& LocalScale() { return m_transform.scale; }

		const Vector2& LocalPosition() const { return m_transform.position; }
		const Rotator& LocalRotation() const { return m_transform.rotation; }
		const Vector2& LocalScale() const { return m_transform.scale; }

		template <typename Comp>
		Ref<Comp> GetComponent()
		{
			static_assert(std::is_base_of_v<Component, Comp>, "GetComponent Type must be of type Component");
			Comp tmp{};
			for (auto& sp_comp : m_components)
			{
				if (sp_comp->TypeID() == tmp.TypeID())
				{
					return Ref<Comp>(sp_comp);
				}
			}
			return Ref<Comp>();
		}

		template <typename Comp>
		Ref<Comp> FindObjectOfType()
		{
			return SceneManager::ActiveScene().FindObjectOfType<Comp>();
		}

		template <typename Comp>
		std::vector<Ref<Comp>> FindObjectsOfType()
		{
			return SceneManager::ActiveScene().FindObjectsOfType<Comp>();
		}

		template <typename Comp>
		Ref<Comp> AddComponent()
		{
			static_assert(std::is_base_of_v<Component, Comp>, "AddComponent Type must be of type Component");
			std::shared_ptr<Component> ptr = m_components.emplace_back(std::make_shared<Comp>());
			return Ref<Comp>(ptr);
		}

		void DestroyComponent(const Ref<Component>& component);

	private:
		friend class Scene;
		void Initialize(Ref<GameObject>& self);

		void OnEditorStart();
		void OnEditorUpdate();
		void OnEditorShutdown();

		void Awake();
		void Start();
		void Update();
		void LateUpdate();
		void OnDestroy();

		void OnEnable();
		void OnDisable();

		friend class SceneManager;
		void Shutdown();

		void CheckComponentBuffers();

		std::string m_name = "GameObject";
		bool m_isActive = true;

		friend class SceneManager;
		uint32_t m_instID = NULL;

		bool m_initialized = false;

		using Components = std::vector<std::shared_ptr<Component>>;
		Components m_components;
		Components m_addBuffer;
		std::vector<Ref<Component>> m_destroyBuffer;

		//Ref<GameObject> m_parent;

		Transform m_transform;


		
	};
}

#endif //!__XEPH2D_GAMEOBJECT_H__
