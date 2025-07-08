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
	// �ӽ÷� newBehaviours�� ������ �����̳� (std::set�� ȿ����)
	std::vector<Behaviour*> newBehaviours;
	std::vector<Behaviour*> changedBehavioursToProcess; // CallBehaviourMessage�� ���� �ӽ� �����̳�

	// m_inactiveBehaviours�� ��ȸ�ϸ� Ȱ��ȭ ��ü ó��
	auto it = m_inactiveBehaviours.begin();
	while (it != m_inactiveBehaviours.end())
	{
		Behaviour* currentBehaviour = *it;
		if (currentBehaviour->IsActiveAndEnabled())
		{
			m_activeBehaviours.push_back(currentBehaviour);
			changedBehavioursToProcess.push_back(currentBehaviour); // OnEnable ȣ���� ���� �߰�

			// m_firstCallBehaviours���� ã��, newBehavioursSet�� �߰� �� m_firstCallBehaviours���� ����
			if (m_firstCallBehaviours.erase(currentBehaviour) > 0) // ��Ұ� ���������� ���ŵǸ�
			{
				newBehaviours.push_back(currentBehaviour); // ���ο� Behaviour�� ����
			}
			m_needSort = true;

			// m_inactiveBehaviours���� ���� ��Ҹ� ����
			it = m_inactiveBehaviours.erase(it);
		}
		else
		{
			++it;
		}
	}

	// Awake ȣ�� (newBehaviours ���)
	for (auto& behaviour : newBehaviours)
	{
		behaviour->CallMessage("Awake");
	}

	// OnEnable ȣ�� (changedBehavioursToProcess ���)
	for (auto& behaviour : changedBehavioursToProcess)
	{
		behaviour->CallMessage("OnEnable");
	}

	// Start ȣ�� (newBehaviours ���)
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

			m_inactiveBehaviours.push_back(currentBehaviour); // �ٷ� m_inactiveBehaviours�� �̵�
			it = m_activeBehaviours.erase(it); // m_activeBehaviours���� ����
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


