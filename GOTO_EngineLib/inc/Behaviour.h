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

	//루프 제어에 영향받는 컴포넌트입니다.
	class Behaviour : public Component
	{
	private:
		friend class BehaviourManager; // BehaviourManager가 접근할 수 있도록
		friend class ObjectDestructionManager; // ObjectDestructionManager가 접근할 수 있도록

		std::unordered_map <std::string, BehaviourMessageData> m_behaviourMessages; // 함수 이름과 함수 포인터를 저장하는 벡터

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
		int m_executionOrder = 0; // 실행 순서를 나타내는 변수

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
