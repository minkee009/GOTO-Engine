#pragma once
#include <Singleton.h>
#include <unordered_map>
#include <string>
#include <queue>
#include <algorithm>

namespace GOTOEngine
{
	class BehaviourManager : public Singleton<BehaviourManager>
	{
	private:
		friend class Engine;
		friend class Behaviour;

		bool m_needSort = false; // Behaviour 정렬이 필요한지 여부
		std::vector<Behaviour*> m_activeBehaviours; // 활성화된 Behaviour를 저장하는 벡터
		std::vector<Behaviour*> m_inactiveBehaviours; // 비활성화된 Behaviour를 저장하는 벡터

		// Behaviour를 등록하는 함수
		void RegisterBehaviour(Behaviour* behaviour);

		// Behaviour를 제거하는 함수
		void UnregisterBehaviour(Behaviour* behaviour);

		// ExcutionOrder에 따라 Behaviour를 정렬하는 함수
		void SortBehaviours();

		void InitializeBehaviours();

		void BroadCastBehaviourMessage(const std::string& messageName);

		// 매개변수 있는 브로드캐스트
		template<typename... Args>
		void BroadCastBehaviourMessage(const std::string& messageName, Args... args)
		{
			for (auto& behaviour : m_activeBehaviours)
			{
				behaviour->CallBehaviourMessage(messageName, args...);
			}
		}

		void CheckAndSortBehaviours();
		void StartUp();
		void ShutDown();
	};
}