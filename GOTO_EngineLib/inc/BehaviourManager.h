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

		std::vector<Behaviour*> m_activeBehaviours; // 활성화된 Behaviour를 저장하는 벡터
		std::vector<Behaviour*> m_inactiveBehaviours; // 비활성화된 Behaviour를 저장하는 벡터

		// Behaviour가 실행대기 시킨 함수들을 저장하는 큐
		std::queue<std::function<void()>> m_pendingFunctions;

		// Behaviour를 등록하는 함수
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
			m_needSort = true; // Behaviour 정렬이 필요함을 표시
		}

		// ExcutionOrder에 따라 Behaviour를 정렬하는 함수
		void SortBehaviours()
		{
			std::sort(m_activeBehaviours.begin(), m_activeBehaviours.end(),
				[](Behaviour* a, Behaviour* b) {
					return a->m_executionOrder < b->m_executionOrder;
				});
		}

		bool m_needSort = false; // Behaviour 정렬이 필요한지 여부

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