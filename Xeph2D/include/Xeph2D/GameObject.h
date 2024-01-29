#ifndef __XEPH2D_GAMEOBJECT_H__
#define __XEPH2D_GAMEOBJECT_H__

#include "Xeph2D/Structs.h"

#include <memory>
#include <string>
#include <vector>

namespace Xeph2D
{
	template <typename ComponentType>
	class Ref;

	class Component;
	class GameObject final
	{
	public:
		uint32_t InstID() { return m_instID; }

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

	private:
		friend class Scene;
		void Initialize(Ref<GameObject>& self);

		void Awake();
		void Start();
		void Update();
		void LateUpdate();
		void OnDestroy();

		void OnEnable();
		void OnDisable();

		friend class SceneLoader;
		void Shutdown();

		std::string m_name = "GameObject";
		bool m_isActive = true;

		friend class SceneLoader;
		uint32_t m_instID = NULL;

		using Components = std::vector<std::shared_ptr<Component>>;
		Components m_components;

		//Ref<GameObject> m_parent;

		Transform m_transform;
		
	};
}

#endif //!__XEPH2D_GAMEOBJECT_H__
