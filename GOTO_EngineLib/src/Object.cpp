#include "Object.h"
#include "TimeManager.h"
#include "ObjectDestructionManager.h"
#include "Mathf.h"

std::atomic<long long> GOTOEngine::Object::s_nextInstanceId = 0;
std::vector<GOTOEngine::Object*> GOTOEngine::Object::s_registry;
std::unordered_set<GOTOEngine::Object*> GOTOEngine::Object::s_validObjects;

//�޸� ����ȭ/������ȭ�� �����ؾ� ��
//������ ����ȭ ���̺귯���� ���� ������ ����
//GameEngine::Object* GameEngine::Object::Instantiate(const Object& src)
//{
//	return new Object(src.name);
//}

void GOTOEngine::Object::Destroy(Object* obj, float delay)
{
	if (Mathf::Approximately(0.0f, delay))
		ObjectDestructionManager::Get()->ImmediateDestroy(obj);
	else
		ObjectDestructionManager::Get()->ScheduleDestroy(obj, delay);
}

void GOTOEngine::Object::DestroyImmediate(Object* obj)
{
	ObjectDestructionManager::Get()->ImmediateDestroy(obj);
}

void GOTOEngine::Object::DontDestroyOnLoad(Object* obj)
{
	//GameObject�� ��� �� ��ü�� ������ �ѱ��
	if (auto go = dynamic_cast<GameObject*>(obj))
	{
		if (go->IsDestroyed() || go->m_scene == SceneManager::Get()->m_dontDestroyOnLoadScene)
			return;

		go->m_scene->UnregisterGameObject(go);
		SceneManager::Get()->m_dontDestroyOnLoadScene->RegisterGameObject(go);
		go->m_scene = SceneManager::Get()->m_dontDestroyOnLoadScene;
	}

	//Component�� ��� ������ GameObject�� ������ �ѱ��
	else if (auto comp = dynamic_cast<Component*>(obj))
	{
		auto go = comp->GetGameObject();
		if (comp->IsDestroyed()
			|| go->IsDestroyed()
			|| go->m_scene == SceneManager::Get()->m_dontDestroyOnLoadScene)
			return;

		go->m_scene->UnregisterGameObject(go);
		SceneManager::Get()->m_dontDestroyOnLoadScene->RegisterGameObject(go);
		go->m_scene = SceneManager::Get()->m_dontDestroyOnLoadScene;
	}
}
