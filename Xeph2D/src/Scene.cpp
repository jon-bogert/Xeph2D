#include "Xeph2D/Scene.h"

#include "Xeph2D/GameObject.h"

void Xeph2D::Scene::Update()
{
#ifdef _EDITOR
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
#endif //_EDITOR
}

void Xeph2D::Scene::Initialize()
{
#ifdef _EDITOR
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
#endif //_EDITOR
}

void Xeph2D::Scene::Shutdown()
{
	for (auto& goPtr : m_gameObjects)
	{
		goPtr->Shutdown();
	}
}
