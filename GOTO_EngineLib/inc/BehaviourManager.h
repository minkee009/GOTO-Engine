#pragma once
#include <Singleton.h>
#include <unordered_map>
#include <string>
#include <queue>
#include <algorithm>
#include "Behaviour.h"

namespace GOTOEngine
{
	class BehaviourManager : public Singleton<BehaviourManager>
	{
	private:
		friend class Engine;
		void BroadCastBehaviourMessage(const std::string& messageName);

		std::vector<Behaviour*> m_activeBehaviours; // Ȱ��ȭ�� Behaviour�� �����ϴ� ����
		std::vector<Behaviour*> m_inactiveBehaviours; // ��Ȱ��ȭ�� Behaviour�� �����ϴ� ����

		// Behaviour�� ������ ��Ų �Լ����� �����ϴ� ť
		std::queue<std::function<void()>> m_pendingFunctions;

		// Behaviour�� ����ϴ� �Լ�
		void RegisterBehaviour(Behaviour* behaviour)
		{
			if (behaviour->GetEnabled())
			{
				m_activeBehaviours.push_back(behaviour);
			}
			else
			{
				m_inactiveBehaviours.push_back(behaviour);
			}
			m_needSort = true; // Behaviour ������ �ʿ����� ǥ��
		}

		// ExcutionOrder�� ���� Behaviour�� �����ϴ� �Լ�
		void SortBehaviours()
		{
			std::sort(m_activeBehaviours.begin(), m_activeBehaviours.end(),
				[](Behaviour* a, Behaviour* b) {
					return a->m_executionOrder < b->m_executionOrder;
				});
		}

		bool m_needSort = false; // Behaviour ������ �ʿ����� ����

		void CheckAndSortBehaviours()
		{
			if (m_needSort)
			{
				SortBehaviours();
				m_needSort = false;
			}
		}

	public:
		void StartUp();
		void ShutDown();
	};
}