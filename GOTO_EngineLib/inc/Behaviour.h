#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Delegate.h"
#include "BehaviourManager.h"
#include <functional>
#include <any>
#include <typeindex>

namespace GOTOEngine
{
	class Behaviour;
	struct BehaviourMessageData
	{
		Behaviour* owner;
		std::string messageName;
		std::function<void()> func;
	};

	//���� ��� ����޴� ������Ʈ�Դϴ�.
	class Behaviour : public Component
	{
	private:
		friend class BehaviourManager;
		friend class GameObject;

		std::unordered_map <std::string, BehaviourMessageData> m_behaviourMessages; // �Լ� �̸��� �Լ� �����͸� �����ϴ� ����

		void CallBehaviourMessage(const std::string& messageName);

	protected:
		Behaviour() : Component()
		{
			m_enabled = true;
			BehaviourManager::Get()->RegisterBehaviour(this); // BehaviourManager�� ���
		}

		bool m_enabled;
		int m_executionOrder = 0; // ���� ������ ��Ÿ���� ����

		void RegisterBehaviourMessage(const std::string& messageName, std::function<void()> func);
		void UnregisterBehaviourMessage(const std::string& messageName);

		void SetExecutionOrder(int order) { m_executionOrder = order; }

		virtual ~Behaviour() 
		{
			BehaviourManager::Get()->UnregisterBehaviour(this); // BehaviourManager���� ����
			m_behaviourMessages.clear();
		}

	public:
		bool GetEnabled() const { return m_enabled; }
		void SetEnabled(bool value);

		bool IsActiveAndEnabled();
	};
}
