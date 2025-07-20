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
	// GameObject�� ��� �� ��ü�� ������ �ѱ��
	if (auto go = dynamic_cast<GameObject*>(obj))
	{
		// �̹� �ı��Ǿ��ų� �̹� DontDestroyOnLoad ���� ������ ó������ ����
		if (go->IsDestroyed() || go->m_scene == SceneManager::Get()->m_dontDestroyOnLoadScene)
			return;

		// �θ� �ִ� ��� �θ� ����
		go->GetTransform()->SetParent(nullptr);

		std::vector<GameObject*> gameObjectsToProcess; 
		gameObjectsToProcess.push_back(go);

		// BFS (�ʺ� �켱 Ž��) ������� ���� ������ ��ȸ�Ͽ� ���� �����÷ο� ����
		while (!gameObjectsToProcess.empty()) 
		{
			GameObject* currentGo = gameObjectsToProcess.back(); 
			gameObjectsToProcess.pop_back(); 

			// �̹� ó���߰ų� �ı��Ǿ��ų� DontDestroyOnLoad ���� ������ �ǳʶ�
			if (currentGo->IsDestroyed() || currentGo->m_scene == SceneManager::Get()->m_dontDestroyOnLoadScene)
				continue;

			// �ڽ��� ���� ������ �����ϰ� DontDestroyOnLoad ���� ���
			if (currentGo->m_scene) // ���� ��ȿ���� Ȯ��
			{
				currentGo->m_scene->UnregisterGameObject(currentGo);
			}
			SceneManager::Get()->m_dontDestroyOnLoadScene->RegisterGameObject(currentGo);
			currentGo->m_scene = SceneManager::Get()->m_dontDestroyOnLoadScene; 

			// �ڽĵ��� ť�� �߰��Ͽ� ���� �ݺ����� ó��
			for (size_t i = 0; i < currentGo->GetTransform()->GetChildCount(); ++i)
			{
				if (auto childGo = currentGo->GetTransform()->GetChild(i)->GetGameObject())
				{
					gameObjectsToProcess.push_back(childGo);
				}
			}
		}
	}



	//Component�� ��� ������ GameObject�� ������ �ѱ��
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
