#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"

using namespace GOTOEngine;

void Scene::RegisterGameObject(GameObject* go)
{
	m_gameObjects.push_back(go);
}

//������ �������� �ʴ� ���Ź�� -> ������ �������
void Scene::UnregisterGameObject(GameObject* go)
{
	auto it = std::find(m_gameObjects.begin(), m_gameObjects.end(), go);
	if (it != m_gameObjects.end()) {
		*it = std::move(m_gameObjects.back()); // ������ ���Ҹ� ���
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
	//iterator ���� ���� -> UnregisterGameObject()ȣ�� ����
	for (auto& go : m_gameObjects)
	{
		go->m_scene = nullptr;
	}
	
	//���� �ı�
	for (auto& go : m_gameObjects)
	{
		if (!Object::IsValidObject(go))
			continue;
		Object::DestroyImmediate(go);
	}

	m_gameObjects.clear();
}
