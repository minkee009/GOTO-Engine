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
	m_needSort = true; // Behaviour ������ �ʿ����� ǥ��
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
	m_needSort = true; // Behaviour ������ �ʿ����� ǥ��
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

	//Ȱ��ȭ ��ü ����
	for (auto& behaviour : m_inactiveBehaviours)
	{
		if (behaviour->IsActiveAndEnabled())
		{
			m_activeBehaviours.push_back(behaviour);
			changedBehaviours.push_back(behaviour);
		}
	}

	//Awake ȣ��
	for (auto& behaviour : changedBehaviours)
	{
		behaviour->CallBehaviourMessage("Awake");
	}

	//OnEnable ȣ��
	for (auto& behaviour : changedBehaviours)
	{
		behaviour->CallBehaviourMessage("OnEnable");
	}

	//Start ȣ��
	for (auto& behaviour : changedBehaviours)
	{
		behaviour->CallBehaviourMessage("Start", true);
	}

	//changedBehaviours�� ��Ҹ� m_inactiveBehaviours���� ����
	for (auto& behaviour : changedBehaviours)
	{
		auto it = std::find(m_inactiveBehaviours.begin(), m_inactiveBehaviours.end(), behaviour);
		if (it != m_inactiveBehaviours.end())
		{
			m_inactiveBehaviours.erase(it);
		}
	}

	m_needSort = true; // Behaviour ������ �ʿ����� ǥ��
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


