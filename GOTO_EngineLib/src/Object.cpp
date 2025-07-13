#include "Object.h"
#include "TimeManager.h"
#include "ObjectDestructionManager.h"
#include "Mathf.h"

std::atomic<long long> GOTOEngine::Object::s_nextInstanceId = 0;
std::vector<GOTOEngine::Object*> GOTOEngine::Object::s_registry;
std::unordered_set<GOTOEngine::Object*> GOTOEngine::Object::s_validObjects;

//메모리 직렬화/역직렬화로 복사해야 함
//연결한 직렬화 라이브러리가 없기 때문에 보류
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
	//GameObject인 경우 그 자체를 씬에게 넘기기
	if (auto go = dynamic_cast<GameObject*>(obj))
	{
		if (go->IsDestroyed() || go->m_scene == SceneManager::Get()->m_dontDestroyOnLoadScene)
			return;

		go->m_scene->UnregisterGameObject(go);
		SceneManager::Get()->m_dontDestroyOnLoadScene->RegisterGameObject(go);
		go->m_scene = SceneManager::Get()->m_dontDestroyOnLoadScene;
	}

	//Component인 경우 소유자 GameObject를 씬에게 넘기기
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
