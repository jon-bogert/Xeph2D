#include "Xeph2D/Scene.h"

#include "Xeph2D/GameObject.h"

Xeph2D::Ref<Xeph2D::GameObject> Xeph2D::Scene::CreateObject()
{
	std::shared_ptr<GameObject>& obj = m_addBuffer.emplace_back(std::make_shared<GameObject>());
	obj->m_instID = Math::Random::UInt32();
	return Ref<GameObject>(obj);
}

void Xeph2D::Scene::DestroyObject(const Ref<GameObject>& object)
{
	m_destroyBuffer.push_back(object);
}

void Xeph2D::Scene::Update()
{
#ifdef IS_EDITOR
	for (auto& goPtr : m_gameObjects)
	{
		goPtr->OnEditorUpdate();
	}
#else
	for (auto& goPtr : m_gameObjects)
	{
		if (goPtr->IsActive())
			goPtr->Update();
	}
	for (auto& goPtr : m_gameObjects)
	{
		if (goPtr->IsActive())
			goPtr->LateUpdate();
	}
#endif //IS_EDITOR
	CheckObjectBuffers();
}

void Xeph2D::Scene::Initialize()
{
	CheckObjectBuffers();
	for (auto& goPtr : m_addBuffer)
	{
		goPtr->Initialize(Ref<GameObject>(goPtr));
	}
#ifdef IS_EDITOR
	for (auto& goPtr : m_gameObjects)
	{
		goPtr->OnEditorStart();
	}
#else
	for (auto& goPtr : m_gameObjects)
	{
		goPtr->Awake();
	}
	for (auto& goPtr : m_gameObjects)
	{
		goPtr->Start();
	}
#endif //IS_EDITOR
	m_initialized = true;
}

void Xeph2D::Scene::Shutdown()
{
	for (auto& goPtr : m_gameObjects)
	{
		goPtr->Shutdown();
	}
}

void Xeph2D::Scene::CheckObjectBuffers()
{
	for (auto& goPtr : m_addBuffer)
	{
		if (m_initialized)
		{
			goPtr->Initialize(Ref<GameObject>(goPtr));
#ifndef IS_EDITOR
			goPtr->Awake();
#endif //!IS_EDITOR
		}
	}
	for (auto& goPtr : m_addBuffer)
	{
		if (m_initialized)
		{
#ifdef IS_EDITOR
			goPtr->OnEditorStart();
#else
			goPtr->Start();
#endif //IS_EDITOR
		}
		m_gameObjects.push_back(goPtr);
	}
	m_addBuffer.clear();

	for (Ref<GameObject>& go : m_destroyBuffer)
	{
#ifdef IS_EDITOR
		go->OnEditorShutdown();
#else
		go->Shutdown();
#endif //IS_EDITOR
		auto iter = std::find_if(
			m_gameObjects.begin(),
			m_gameObjects.end(),
			[&](std::shared_ptr<GameObject>& ptr) { return Ref<GameObject>(ptr) == go; }
		);
		if (iter == m_gameObjects.end())
		{
			Debug::LogErr("Scene::CheckObjectBuffers -> Could not find object to destroy");
			return;
		}
		m_gameObjects.erase(iter);
	}
	m_destroyBuffer.clear();
}
