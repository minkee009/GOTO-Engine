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

	//루프 제어에 영향받는 컴포넌트입니다.
	class Behaviour : public Component
	{
	private:
		friend class BehaviourManager;
		friend class GameObject;

		std::unordered_map <std::string, BehaviourMessageData> m_behaviourMessages; // 함수 이름과 함수 포인터를 저장하는 벡터

		void CallBehaviourMessage(const std::string& messageName);

	protected:
		Behaviour() : Component()
		{
			m_enabled = true;
			BehaviourManager::Get()->RegisterBehaviour(this); // BehaviourManager에 등록
		}

		bool m_enabled;
		int m_executionOrder = 0; // 실행 순서를 나타내는 변수

		void RegisterBehaviourMessage(const std::string& messageName, std::function<void()> func);
		void UnregisterBehaviourMessage(const std::string& messageName);

		void SetExecutionOrder(int order) { m_executionOrder = order; }

		virtual ~Behaviour() 
		{
			BehaviourManager::Get()->UnregisterBehaviour(this); // BehaviourManager에서 제거
			m_behaviourMessages.clear();
		}

	public:
		bool GetEnabled() const { return m_enabled; }
		void SetEnabled(bool value);

		bool IsActiveAndEnabled();
	};
}
