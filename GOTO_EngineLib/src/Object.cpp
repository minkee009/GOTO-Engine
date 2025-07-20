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
	//if (Mathf::Approximately(0.0f, delay))
	//	ObjectDestructionManager::Get()->ImmediateDestroy(obj);
	//else
		ObjectDestructionManager::Get()->ScheduleDestroy(obj, delay);
}

void GOTOEngine::Object::DestroyImmediate(Object* obj)
{
	ObjectDestructionManager::Get()->ImmediateDestroy(obj);
}

void GOTOEngine::Object::DontDestroyOnLoad(Object* obj)
{
	// GameObject인 경우 그 자체를 씬에게 넘기기
	if (auto go = dynamic_cast<GameObject*>(obj))
	{
		// 이미 파괴되었거나 이미 DontDestroyOnLoad 씬에 있으면 처리하지 않음
		if (go->IsDestroyed() || go->m_scene == SceneManager::Get()->m_dontDestroyOnLoadScene)
			return;

		// 부모가 있는 경우 부모를 끊기
		go->GetTransform()->SetParent(nullptr);

		std::vector<GameObject*> gameObjectsToProcess; 
		gameObjectsToProcess.push_back(go);

		// BFS (너비 우선 탐색) 방식으로 계층 구조를 순회하여 스택 오버플로우 방지
		while (!gameObjectsToProcess.empty()) 
		{
			GameObject* currentGo = gameObjectsToProcess.back(); 
			gameObjectsToProcess.pop_back(); 

			// 이미 처리했거나 파괴되었거나 DontDestroyOnLoad 씬에 있으면 건너뜀
			if (currentGo->IsDestroyed() || currentGo->m_scene == SceneManager::Get()->m_dontDestroyOnLoadScene)
				continue;

			// 자신을 현재 씬에서 해제하고 DontDestroyOnLoad 씬에 등록
			if (currentGo->m_scene) // 씬이 유효한지 확인
			{
				currentGo->m_scene->UnregisterGameObject(currentGo);
			}
			SceneManager::Get()->m_dontDestroyOnLoadScene->RegisterGameObject(currentGo);
			currentGo->m_scene = SceneManager::Get()->m_dontDestroyOnLoadScene; 

			// 자식들을 큐에 추가하여 다음 반복에서 처리
			for (size_t i = 0; i < currentGo->GetTransform()->GetChildCount(); ++i)
			{
				if (auto childGo = currentGo->GetTransform()->GetChild(i)->GetGameObject())
				{
					gameObjectsToProcess.push_back(childGo);
				}
			}
		}
	}



	//Component인 경우 소유자 GameObject를 씬에게 넘기기
	else if (auto comp = dynamic_cast<Component*>(obj))
	{
		auto go = comp->GetGameObject();
		if (comp->IsDestroyed()
			|| go->IsDestroyed()
			|| go->m_scene == SceneManager::Get()->m_dontDestroyOnLoadScene)
			return;

		DontDestroyOnLoad(go);
	}
}
