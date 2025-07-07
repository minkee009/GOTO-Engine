#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Delegate.h"

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
		friend class BehaviourManager; // BehaviourManager�� ������ �� �ֵ���
		friend class ObjectDestructionManager; // ObjectDestructionManager�� ������ �� �ֵ���

		std::unordered_map <std::string, BehaviourMessageData> m_behaviourMessages; // �Լ� �̸��� �Լ� �����͸� �����ϴ� ����

		void CallBehaviourMessage(const std::string& messageName)
		{
			auto it = m_behaviourMessages.find(messageName);
			if (it != m_behaviourMessages.end())
			{
				it->second.func();
			}
		}

	protected:
		bool m_enabled;
		int m_executionOrder = 0; // ���� ������ ��Ÿ���� ����

		void RegisterBehaviourMessage(const std::string& messageName, std::function<void()> func)
		{
			BehaviourMessageData data;
			data.owner = this;
			data.messageName = messageName;
			data.func = func;
			m_behaviourMessages.insert({ messageName, data });
		}

		void UnregisterBehaviourMessage(const std::string& messageName)
		{
			auto it = m_behaviourMessages.find(messageName);
			if (it != m_behaviourMessages.end())
			{
				m_behaviourMessages.erase(it);
			}
		}

		void SetExecutionOrder(int order)
		{
			m_executionOrder = order;
		}

		virtual ~Behaviour() {}
	public:
		bool GetEnabled() const { return m_enabled; }
		void SetEnabled(bool value);

		bool IsActiveAndEnabled();
	};
}
