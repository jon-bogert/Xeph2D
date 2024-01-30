#include "Xeph2D/Systems/RenderStack.h"

#include "Xeph2D/Systems/WindowManager.h"
#include "Xeph2D/Systems/Debug.h"
#include "Xeph2D/GameObject.h"

using namespace Xeph2D;

RenderStack& RenderStack::Get()
{
	static RenderStack instance;
	return instance;
}

void RenderStack::AddDrawable(const Ref<GameObject>& gameObject, sf::Transformable* transformable, sf::Drawable* drawable, int order)
{
	Item item;
	item.order = order;
	item.object = gameObject;
	item.transfromable = transformable;
	item.drawable = drawable;

	Get().m_stack.push_back(item);
	std::sort(Get().m_stack.begin(), Get().m_stack.end(), [](const Item& i0, const Item& i1) { return i0.order < i1.order; });
}

void RenderStack::Draw()
{
	for (Callback& callback : Get().m_drawCallbacks)
	{
		callback.func();
	}

	for (Item& i : Get().m_stack)
	{
		WindowManager::PrepareTransformable(i.object, i.transfromable);
		WindowManager::Draw(i.drawable);
	}
	Get().m_stack.clear();
}

void Xeph2D::RenderStack::SubscribeDrawCall(void* component, std::function<void(void)> drawCall)
{
	Get().m_drawCallbacks.push_back({ component, drawCall });
}

void Xeph2D::RenderStack::UnubscribeDrawCall(void* component)
{
	for (auto it = Get().m_drawCallbacks.begin(); it != Get().m_drawCallbacks.end();)
	{
		if (it->comp == component)
			it = Get().m_drawCallbacks.erase(it);
		else
			it++;
	}
}
