#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"

using namespace GOTOEngine;

void Scene::RegisterGameObject(GameObject* go)
{
	m_gameObjects.push_back(go);
}

//순서를 보장하지 않는 제거방식 -> 하지만 가장빠름
void Scene::UnregisterGameObject(GameObject* go)
{
	auto it = std::find(m_gameObjects.begin(), m_gameObjects.end(), go);
	if (it != m_gameObjects.end()) {
		*it = std::move(m_gameObjects.back()); // 마지막 원소를 덮어씀
		m_gameObjects.pop_back();
	}
}

GameObject* GOTOEngine::Scene::CreateGameObject(std::wstring name)
{
	auto go = new GameObject(this, name);
	return go;
}

GameObject* GOTOEngine::Scene::CreateGameObject()
{
	auto go = new GameObject(this);
	return go;
}

void Scene::Clear()
{
	//iterator 오염 방지 -> UnregisterGameObject()호출 방지
	for (auto& go : m_gameObjects)
	{
		go->m_scene = nullptr;
	}
	
	//실제 파괴
	for (auto& go : m_gameObjects)
	{
		if (!Object::IsValidObject(go))
			continue;
		Object::DestroyImmediate(go);
	}

	m_gameObjects.clear();
}
