#ifndef __XEPH2D_REF_H__
#define __XEPH2D_REF_H__

#include "Xeph2D/Systems/Debug.h"

#include <type_traits>
#include <memory>

namespace Xeph2D
{
	class Component;
	class GameObject;

	template <typename ComponentType>
	class Ref final
	{
	public:
		Ref()
		{
			static_assert(std::is_base_of_v<Component, ComponentType> || std::is_base_of_v<GameObject, ComponentType>, "Ref Component Type must inherit from Component or be a GameObject");
			m_ptr.reset();
		}
		constexpr Ref(std::shared_ptr<ComponentType>& ptr)
		{
			static_assert(std::is_base_of_v<Component, ComponentType> || std::is_base_of_v<GameObject, ComponentType>, "Ref Component Type must inherit from Component or be a GameObject");
			m_ptr = ptr;
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
			return !m_ptr.expired();
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
			return m_ptr.expired();
		}

		bool operator!=(Ref<ComponentType>& other) const
		{
			return other.m_ptr.lock().get() != m_ptr.lock().get();
		}

		Ref<ComponentType>& operator=(const Ref<ComponentType>&) = default;
		Ref<ComponentType>& operator=(const std::shared_ptr<ComponentType>& ptr)
		{
			static_assert(std::is_base_of_v<Component, ComponentType> || std::is_base_of_v<GameObject, ComponentType>, "Ref Component Type must inherit from Component or be a GameObject");
			m_ptr = ptr;
		}

		Ref<ComponentType>& operator=(const ComponentType* ptr)
		{
			if (ptr != nullptr)
			{
				Debug::LogWarn("Ref -> cannot set Ref to raw pointer other than nullptr");
				return *this;
			}
			m_ptr.reset();
		}

	private:
		std::weak_ptr<ComponentType> m_ptr {};
	};
}

#endif //!__XEPH2D_REF_H__