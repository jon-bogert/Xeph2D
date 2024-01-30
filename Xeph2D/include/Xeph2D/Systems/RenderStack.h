#ifndef __X2D_SYSTEMS_RENDERSTACK_H__
#define __X2D_SYSTEMS_RENDERSTACK_H__

#include "Xeph2D/Ref.h"

#include <vector>
#include <functional>
#include <SFML.hpp>

namespace Xeph2D
{
	class GameObject;
	class RenderStack final
	{
	public:
		~RenderStack() = default;
		RenderStack(const RenderStack& other) = delete;
		RenderStack(const RenderStack&& other) = delete;
		RenderStack operator=(const RenderStack& other) = delete;
		RenderStack operator=(const RenderStack&& other) = delete;

		static void AddDrawable(const Ref<GameObject>& gameObject, sf::Transformable* transformable, sf::Drawable* drawable, int order = 0);
		static void Draw();

		static void SubscribeDrawCall(void* component, std::function<void(void)> drawCall);
		static void UnubscribeDrawCall(void* component);

	private:
		struct Item
		{
			enum class Type { Sprite };
			int order = 0;
			Ref<GameObject> object;
			sf::Transformable* transfromable = nullptr;
			sf::Drawable* drawable = nullptr;

			Item() = default;
			Item(int order, const Ref<GameObject>& object, Type type, void* data)
				: order(order), object(object), transfromable(transfromable), drawable(drawable) {}
		};

		struct Callback
		{
			void* comp;
			std::function<void(void)> func;
		};

		std::vector<Callback> m_drawCallbacks;
		std::vector<Item> m_stack;

		RenderStack() {}
		static RenderStack& Get();
	};
}

#endif //!__X2D_SYSTEMS_RENDERSTACK_H__