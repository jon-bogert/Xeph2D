#ifndef __XEPH2D_REF_H__
#define __XEPH2D_REF_H__

#include "Xeph2D/Systems/Debug.h"

#include <type_traits>
#include <memory>

namespace Xeph2D
{
	class Component;
	class GameObject;
#ifdef IS_EDITOR
	namespace Edit
	{
		class Editor;
	}
#endif //IS_EDITOR

	template <typename ComponentType>
	class Ref final
	{
#ifdef IS_EDITOR
		friend class Edit::Editor;
#endif //IS_EDITOR
	public:
		Ref()
		{
			static_assert(std::is_base_of_v<Component, ComponentType> || std::is_base_of_v<GameObject, ComponentType>, "Ref Component Type must inherit from Component or be a GameObject");
			m_ptr.reset();
		}
		constexpr Ref(std::shared_ptr<GameObject>& ptr)
		{
			static_assert(std::is_base_of_v<Component, ComponentType> || std::is_base_of_v<GameObject, ComponentType>, "Ref Component Type must inherit from Component or be a GameObject");
			m_ptr = ptr;
		}
		constexpr Ref(std::shared_ptr<Component>& ptr)
		{
			static_assert(std::is_base_of_v<Component, ComponentType> || std::is_base_of_v<GameObject, ComponentType>, "Ref Component Type must inherit from Component or be a GameObject");
			m_ptr = std::dynamic_pointer_cast<ComponentType>(ptr);
		}

		ComponentType& operator*(){ return *m_ptr.lock(); }
		ComponentType& operator*() const { return *m_ptr.lock(); }
		ComponentType* operator->(){ return m_ptr.lock().get(); }
		ComponentType* operator->() const { return m_ptr.lock().get(); }

		bool operator==(ComponentType* other) const
		{
			if (other != nullptr)
			{
				Debug::LogWarn("Ref -> cannot perform comparison with raw pointer");
				return false;
			}
			return m_ptr.expired();
		}

		bool operator==(Ref<ComponentType>& other) const
		{
			return other.m_ptr.lock().get() == m_ptr.lock().get();
		}

		bool operator!=(ComponentType* other) const
		{
			if (other != nullptr)
			{
				Debug::LogWarn("Ref -> cannot perform comparison with raw pointer");
				return true;
			}
			return !m_ptr.expired();
		}

		bool operator!=(Ref<ComponentType>& other) const
		{
			return other.m_ptr.lock().get() != m_ptr.lock().get();
		}

		Ref<ComponentType>& operator=(const Ref<ComponentType>& other)
		{
			static_assert(std::is_base_of_v<Component, ComponentType> || std::is_base_of_v<GameObject, ComponentType>, "Ref Component Type must inherit from Component or be a GameObject");
			m_ptr = other.m_ptr;
			return *this;
		}

		Ref<ComponentType>& operator=(const std::shared_ptr<ComponentType>& ptr)
		{
			static_assert(std::is_base_of_v<Component, ComponentType> || std::is_base_of_v<GameObject, ComponentType>, "Ref Component Type must inherit from Component or be a GameObject");
			m_ptr = ptr;
			return *this;
		}

		Ref<ComponentType>& operator=(const ComponentType* ptr)
		{
			if (ptr != nullptr)
			{
				Debug::LogWarn("Ref -> cannot set Ref to raw pointer other than nullptr");
				return *this;
			}
			m_ptr.reset();
			return *this;
		}

		bool IsNull() const
		{
			return m_ptr.expired();
		}

	private:
		std::weak_ptr<ComponentType> m_ptr {};
	};
}

#endif //!__XEPH2D_REF_H__