#include "Xeph2D/Scene.h"

#include "Xeph2D/GameObject.h"

void Xeph2D::Scene::Update()
{
	for (auto& goPtr : m_gameObjects)
	{
		goPtr->Update();
	}
}

void Xeph2D::Scene::Initialize()
{
	for (auto& goPtr : m_gameObjects)
	{
		goPtr->Awake();
	}
	for (auto& goPtr : m_gameObjects)
	{
		goPtr->Start();
	}
}

void Xeph2D::Scene::Shutdown()
{
	for (auto& goPtr : m_gameObjects)
	{
		goPtr->Shutdown();
	}
}
