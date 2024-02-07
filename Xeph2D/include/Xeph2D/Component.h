#ifndef __XEPH2D_COMPONENT_H__
#define __XEPH2D_COMPONENT_H__

#include "Xeph2D/Events.h"
#include "Xeph2D/Ref.h"
#include "Xeph2D/GameObject.h"

#define XEPH2D_REG_COMP(id)public:\
inline static uint32_t typeID = id;\
uint32_t TypeID() override { return typeID; }\
private:

namespace Xeph2D
{
#ifdef _EDITOR
	namespace Edit
	{
		class Editor;
	}
#endif //_EDITOR
	class Component
	{
#ifdef _EDITOR
		friend class Edit::Editor;
#endif //_EDITOR
	public:
		//Global Transforms
		//Vector2 Position() const { return gameObject->Position(); }
		//Rotator Rotation() const { return gameObject->Rotation(); }
		//Vector2 Scale() const { return gameObject->Scale(); }
		//void SetPosition(const Vector3 position) { return gameObject->SetPosition(position); }
		//void SetRotation(const Rotator rotation) { return gameObject->SetRotation(rotation); };
		//void SetRotation(const float rotation) { return gameObject->SetRotation(rotation); };

		Vector2& LocalPosition() { return gameObject->LocalPosition(); }
		Rotator& LocalRotation() { return gameObject->LocalRotation(); }
		Vector2& LocalScale() { return gameObject->LocalScale(); }

		bool Enabled() const { return m_enabled; }
		void SetEnabled(const bool enabled)
		{
			m_enabled = enabled;
			if (m_enabled)
				OnEnable();
			else
				OnDisable();
		}

		bool IsActiveAndEnabled() const
		{
			return gameObject->IsActive() && m_enabled;
		}

		template <typename Comp>
		Ref<Comp> GetComponent()
		{
			return gameObject->GetComponent<Comp>();
		}

		template <typename Comp>
		Ref<Comp> FindObjectOfType()
		{
			return SceneManager::ActiveScene().FindObjectOfType<Comp>();
		}

	protected:
		friend class GameObject;
		virtual void Serializables() {}

		virtual uint32_t TypeID() = 0;

		virtual void Awake() {}
		virtual void Start() {}
		virtual void OnEnable() {}
		virtual void Update() {}
		virtual void LateUpdate() {}
		virtual void OnDisable() {}
		virtual void OnDestroy() {}

		virtual void OnEditorStart() {}
		virtual void OnEditorUpdate() {}
		virtual void OnEditorShutdown() {}

		Ref<GameObject> gameObject;

	private:
		friend class SceneManager;

		template <typename ComponentType>
		friend class Ref; // and GameObject

		bool m_enabled = true;
	};
}

#endif //__XEPH2D_COMPONENT_H__