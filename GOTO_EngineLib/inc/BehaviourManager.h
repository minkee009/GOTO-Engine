#pragma once
#include <Singleton.h>
#include <unordered_set>
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
		friend class Behaviour;

		bool m_needSort = false; // Behaviour ������ �ʿ����� ����
		std::vector<Behaviour*> m_activeBehaviours; // Ȱ��ȭ�� Behaviour�� �����ϴ� ����
		std::vector<Behaviour*> m_inactiveBehaviours; // ��Ȱ��ȭ�� Behaviour�� �����ϴ� ����
		std::unordered_set<Behaviour*> m_firstCallBehaviours;

		// Behaviour�� ����ϴ� �Լ�
		void RegisterBehaviour(Behaviour* behaviour);

		// Behaviour�� �����ϴ� �Լ�
		void UnregisterBehaviour(Behaviour* behaviour);

		// ExcutionOrder�� ���� Behaviour�� �����ϴ� �Լ�
		void SortBehaviours();

		void InitializeBehaviours();
		
		// ��Ȱ��ȭ�� Behaviour�� �����ϴ� �Լ�
		void DisableBehaviours();

		void BroadCastBehaviourMessage(const std::string& messageName);

		// �Ű����� �ִ� ��ε�ĳ��Ʈ
		template<typename... Args>
		void BroadCastBehaviourMessage(const std::string& messageName, Args... args)
		{
			std::vector<std::any> params;
			params.reserve(sizeof...(args)); // ���� ����ȭ�� ���� �̸� ũ�� ����
			(params.emplace_back(args), ...); // C++17 fold expression�� ����Ͽ� �� ���ڸ� vector�� �߰�

			for (auto& behaviour : m_activeBehaviours)
			{
				// ��ȯ�� std::vector<std::any>�� CallBehaviourMessage�� �����մϴ�.
				behaviour->CallBehaviourMessage(messageName, params);
			}
		}

		void CheckAndSortBehaviours();
		void StartUp();
		void ShutDown();
	};
}