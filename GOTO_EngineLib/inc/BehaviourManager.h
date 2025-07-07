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

		bool m_needSort = false; // Behaviour ������ �ʿ����� ����
		std::vector<Behaviour*> m_activeBehaviours; // Ȱ��ȭ�� Behaviour�� �����ϴ� ����
		std::vector<Behaviour*> m_inactiveBehaviours; // ��Ȱ��ȭ�� Behaviour�� �����ϴ� ����

		// Behaviour�� ����ϴ� �Լ�
		void RegisterBehaviour(Behaviour* behaviour);

		// Behaviour�� �����ϴ� �Լ�
		void UnregisterBehaviour(Behaviour* behaviour);

		// ExcutionOrder�� ���� Behaviour�� �����ϴ� �Լ�
		void SortBehaviours();

		void InitializeBehaviours();

		void BroadCastBehaviourMessage(const std::string& messageName);

		// �Ű����� �ִ� ��ε�ĳ��Ʈ
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