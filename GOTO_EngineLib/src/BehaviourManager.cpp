#include "BehaviourManager.h"
#include "Behaviour.h"


void GOTOEngine::BehaviourManager::BroadCastBehaviourMessage(const std::string& funcName)
{
	for (auto& behaviour : m_activeBehaviours)
	{
		behaviour->CallMessage(funcName);
	}
}

void GOTOEngine::BehaviourManager::RegisterBehaviour(Behaviour* behaviour)
{
	m_inactiveBehaviours.push_back(behaviour);
	m_firstCallBehaviours.insert(behaviour);
	m_needSort = true; // Behaviour 정렬이 필요함을 표시
}

void GOTOEngine::BehaviourManager::UnregisterBehaviour(Behaviour* behaviour)
{
	auto it = std::find(m_activeBehaviours.begin(), m_activeBehaviours.end(), behaviour);
	if (it != m_activeBehaviours.end())
	{
		m_activeBehaviours.erase(it);
	}
	else
	{
		it = std::find(m_inactiveBehaviours.begin(), m_inactiveBehaviours.end(), behaviour);
		if (it != m_inactiveBehaviours.end())
		{
			m_inactiveBehaviours.erase(it);
		}
	}
	m_needSort = true; // Behaviour 정렬이 필요함을 표시
}


void GOTOEngine::BehaviourManager::SortBehaviours()
{
	std::sort(m_activeBehaviours.begin(), m_activeBehaviours.end(),
		[](Behaviour* a, Behaviour* b) {
			return a->m_executionOrder < b->m_executionOrder;
		});
}

void GOTOEngine::BehaviourManager::InitializeBehaviours()
{
	// 임시로 newBehaviours를 수집할 컨테이너 (std::set이 효율적)
	std::vector<Behaviour*> newBehaviours;
	std::vector<Behaviour*> changedBehavioursToProcess; // CallBehaviourMessage를 위한 임시 컨테이너

	// m_inactiveBehaviours를 순회하며 활성화 객체 처리
	auto it = m_inactiveBehaviours.begin();
	while (it != m_inactiveBehaviours.end())
	{
		Behaviour* currentBehaviour = *it;
		if (currentBehaviour->IsActiveAndEnabled())
		{
			m_activeBehaviours.push_back(currentBehaviour);
			changedBehavioursToProcess.push_back(currentBehaviour); // OnEnable 호출을 위해 추가

			// m_firstCallBehaviours에서 찾고, newBehavioursSet에 추가 및 m_firstCallBehaviours에서 제거
			if (m_firstCallBehaviours.erase(currentBehaviour) > 0) // 요소가 성공적으로 제거되면
			{
				newBehaviours.push_back(currentBehaviour); // 새로운 Behaviour로 간주
			}
			m_needSort = true;

			// m_inactiveBehaviours에서 현재 요소를 제거
			it = m_inactiveBehaviours.erase(it);
		}
		else
		{
			++it;
		}
	}

	// Awake 호출 (newBehaviours 사용)
	for (auto& behaviour : newBehaviours)
	{
		behaviour->CallMessage("Awake");
	}

	// OnEnable 호출 (changedBehavioursToProcess 사용)
	for (auto& behaviour : changedBehavioursToProcess)
	{
		behaviour->CallMessage("OnEnable");
	}

	// Start 호출 (newBehaviours 사용)
	for (auto& behaviour : newBehaviours)
	{
		behaviour->CallMessage("Start");
	}
}

void GOTOEngine::BehaviourManager::DisableBehaviours()
{
	auto it = m_activeBehaviours.begin();
	while (it != m_activeBehaviours.end())
	{
		Behaviour* currentBehaviour = *it;
		if (!currentBehaviour->IsActiveAndEnabled() || currentBehaviour->Destroyed())
		{
			(*it)->CallMessage("OnDisable");
			m_needSort = true;

			m_inactiveBehaviours.push_back(currentBehaviour); // 바로 m_inactiveBehaviours로 이동
			it = m_activeBehaviours.erase(it); // m_activeBehaviours에서 제거
		}
		else
		{
			++it;
		}
	}
	
	for (auto inactive : m_inactiveBehaviours)
	{
		if (inactive->Destroyed())
		{
			inactive->CallMessage("OnDestroy");
		}
	}
}

void GOTOEngine::BehaviourManager::CheckAndSortBehaviours()
{
	if (m_needSort)
	{
		SortBehaviours();
		m_needSort = false;
	}
}

void GOTOEngine::BehaviourManager::StartUp()
{
}

void GOTOEngine::BehaviourManager::ShutDown()
{
	m_activeBehaviours.clear();
	m_inactiveBehaviours.clear();
}


