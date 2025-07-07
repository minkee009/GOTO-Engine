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

		// Behaviour를 제거하는 함수
		void UnregisterBehaviour(Behaviour* behaviour)
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
		void StartUp();
		void ShutDown();
	};
}