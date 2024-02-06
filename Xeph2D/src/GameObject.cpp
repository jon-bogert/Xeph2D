#include "Xeph2D/GameObject.h"

#include "Xeph2D/Component.h"
#include "Xeph2D/Ref.h"

void Xeph2D::GameObject::SetIsActive(const bool isActive)
{
	m_isActive = isActive;
	if (m_isActive)
		OnEnable();
	else
		OnDisable();
}

void Xeph2D::GameObject::DestroyComponent(const Ref<Component>& component)
{
	m_destroyBuffer.push_back(component);
}

void Xeph2D::GameObject::Initialize(Ref<GameObject>& self)
{
	CheckComponentBuffers();
	for (auto& compPtr : m_components)
		compPtr->gameObject = self;
	m_initialized = true;
}

void Xeph2D::GameObject::OnEditorStart()
{
	for (auto& compPtr : m_components)
		compPtr->OnEditorStart();
}

void Xeph2D::GameObject::OnEditorUpdate()
{
	for (auto& compPtr : m_components)
		compPtr->OnEditorUpdate();
}

void Xeph2D::GameObject::OnEditorShutdown()
{
	for (auto& compPtr : m_components)
		compPtr->OnEditorShutdown();
}

void Xeph2D::GameObject::Awake()
{
	for (auto& compPtr : m_components)
		compPtr->Awake();
}

void Xeph2D::GameObject::Start()
{
	for (auto& compPtr : m_components)
		compPtr->Start();
	for (auto& compPtr : m_components)
	{
		if (compPtr->Enabled())
			compPtr->OnEnable();
	}
}

void Xeph2D::GameObject::Update()
{
	for (auto& compPtr : m_components)
	{
		if (compPtr->Enabled())
			compPtr->Update();
	}
}

void Xeph2D::GameObject::LateUpdate()
{
	for (auto& compPtr : m_components)
	{
		if (compPtr->Enabled())
			compPtr->LateUpdate();
	}
	CheckComponentBuffers();
}

void Xeph2D::GameObject::OnDestroy()
{
	for (auto& compPtr : m_components)
		compPtr->OnDestroy();
}

void Xeph2D::GameObject::OnEnable()
{
	for (auto& compPtr : m_components)
		compPtr->OnEnable();
}

void Xeph2D::GameObject::OnDisable()
{
	for (auto& compPtr : m_components)
		compPtr->OnDisable();
}

void Xeph2D::GameObject::Shutdown()
{
#ifdef _EDITOR
	for (auto& compPtr : m_components)
		compPtr->OnEditorShutdown();
#else
	for (auto& compPtr : m_components)
	{
		if (compPtr->Enabled())
			compPtr->OnDisable();
	}

	for (auto& compPtr : m_components)
		compPtr->OnDestroy();
#endif //_EDITOR
}

void Xeph2D::GameObject::CheckComponentBuffers()
{
	for (auto& compPtr : m_addBuffer)
	{
		if (m_initialized)
		{
#ifndef _EDITOR
			compPtr->Awake();
#endif //!_EDITOR
		}
	}
	for (auto& compPtr : m_addBuffer)
	{
		if (m_initialized)
		{
#ifdef _EDITOR
			compPtr->OnEditorStart();
#else
			compPtr->Start();
			if (compPtr->Enabled())
				compPtr->OnEnable();
#endif //_EDITOR
		}
		m_components.push_back(compPtr);
	}
	m_addBuffer.clear();

	for (Ref<Component>& comp : m_destroyBuffer)
	{
#ifdef _EDITOR
		comp->OnEditorShutdown();
#else
		if (comp->Enabled())
			comp->OnDisable();
		comp->OnDestroy();
#endif //_EDITOR
		auto iter = std::find_if(
			m_components.begin(),
			m_components.end(),
			[&](std::shared_ptr<Component>& ptr) { return Ref<Component>(ptr) == comp; }
		);
		if (iter == m_components.end())
		{
			Debug::LogErr("GameObject::CheckComponentBuffers -> Could not find component to destroy");
			return;
		}
		m_components.erase(iter);
	}
	m_destroyBuffer.clear();
}
