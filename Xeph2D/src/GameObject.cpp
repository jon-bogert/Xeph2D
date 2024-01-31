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

void Xeph2D::GameObject::Initialize(Ref<GameObject>& self)
{
	//remember to set component.gameObject to self;
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
		compPtr->Update();
}

void Xeph2D::GameObject::LateUpdate()
{
	for (auto& compPtr : m_components)
		compPtr->LateUpdate();
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
	for (auto& compPtr : m_components)
	{
		if (compPtr->Enabled())
			compPtr->OnDisable();
	}

	for (auto& compPtr : m_components)
		compPtr->OnDestroy();
}