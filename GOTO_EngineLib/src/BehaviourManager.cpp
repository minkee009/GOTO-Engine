#include "BehaviourManager.h"
#include "Behaviour.h"


void GOTOEngine::BehaviourManager::BroadCastBehaviourMessage(const std::string& funcName)
{
	for (auto& behaviour : m_activeBehaviours)
	{
		behaviour->CallBehaviourMessage(funcName);
	}
}

void GOTOEngine::BehaviourManager::RegisterBehaviour(Behaviour* behaviour)
{
	m_inactiveBehaviours.push_back(behaviour);
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
	std::vector<Behaviour*> changedBehaviours;

	//활성화 객체 수집
	for (auto& behaviour : m_inactiveBehaviours)
	{
		if (behaviour->IsActiveAndEnabled())
		{
			m_activeBehaviours.push_back(behaviour);
			changedBehaviours.push_back(behaviour);
		}
	}

	//Awake 호출
	for (auto& behaviour : changedBehaviours)
	{
		behaviour->CallBehaviourMessage("Awake");
	}

	//OnEnable 호출
	for (auto& behaviour : changedBehaviours)
	{
		behaviour->CallBehaviourMessage("OnEnable");
	}

	//Start 호출
	for (auto& behaviour : changedBehaviours)
	{
		behaviour->CallBehaviourMessage("Start", true);
	}

	//changedBehaviours의 요소를 m_inactiveBehaviours에서 제거
	for (auto& behaviour : changedBehaviours)
	{
		auto it = std::find(m_inactiveBehaviours.begin(), m_inactiveBehaviours.end(), behaviour);
		if (it != m_inactiveBehaviours.end())
		{
			m_inactiveBehaviours.erase(it);
		}
	}

	m_needSort = true; // Behaviour 정렬이 필요함을 표시
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


