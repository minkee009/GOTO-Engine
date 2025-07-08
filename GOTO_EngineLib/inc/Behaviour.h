#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Delegate.h"
#include <functional>
#include <any>
#include <typeindex>

#include <tuple> 
#include <utility> 

namespace GOTOEngine
{
	class Behaviour;

	struct BehaviourMessageData
	{
		Behaviour* owner;
		std::string messageName;
		std::function<void()> func;
	};

	struct BehaviourParamMessageData
	{
		Behaviour* owner;
		std::string messageName;
		std::function<void(const std::vector<std::any>&)> func;
		std::vector<std::type_index> parameterTypes;

		// 기본 생성자
		BehaviourParamMessageData() = default;

		// 생성자 수정: std::function의 실제 인자 타입을 추출하여 사용
		template<typename... Args>
		BehaviourParamMessageData(const std::string& name, Behaviour* owner, std::function<void(Args...)> funcPtr)
			: owner(owner), messageName(name)
		{
			parameterTypes = { std::type_index(typeid(Args))... };

			// 람다로 타입 변환 처리
			// CallWithParameters에 Args... 팩을 직접 전달하여 매개변수 타입을 알립니다.
			func = [funcPtr](const std::vector<std::any>& params) {
				if (params.size() != sizeof...(Args)) {
					throw std::runtime_error("Parameter count mismatch");
				}
				// CallWithParameters 호출 시 Args... 팩을 명시적으로 전달
				CallWithParameters<Args...>(funcPtr, params, std::index_sequence_for<Args...>{});
				};
		}

	private:
		// CallWithParameters의 템플릿 인자를 Func와 함께 Args...로 직접 받습니다.
		template<typename... OriginalArgs, typename Func, std::size_t... I>
		static void CallWithParameters(Func&& funcPtr, const std::vector<std::any>& params, std::index_sequence<I...>) {
			// 이제 OriginalArgs...가 실제 함수 시그니처의 타입 팩이므로
			// std::tuple<OriginalArgs...>를 직접 사용할 수 있습니다.
			using ArgsTuple = std::tuple<OriginalArgs...>;

			funcPtr(std::any_cast<std::tuple_element_t<I, ArgsTuple>>(params[I])...);
		}
	};


	//루프 제어에 영향받는 컴포넌트입니다.
	class Behaviour : public Component
	{
	private:
		friend class BehaviourManager;
		friend class GameObject;

		std::unordered_map <std::string, BehaviourMessageData> m_behaviourMessages; // 함수 이름과 함수 포인터를 저장하는 벡터
		std::unordered_map <std::string, BehaviourParamMessageData> m_behaviourParamMessages; // 함수 이름과 함수 포인터를 저장하는 벡터

		void CallBehaviourMessage(const std::string& messageName);
		void CallBehaviourMessage(const std::string& messageName, const std::vector<std::any>& params);

	protected:
		Behaviour();

		bool m_enabled;
		int m_executionOrder = 0; // 실행 순서를 나타내는 변수

		// 매개변수 없는 함수 등록
		void RegisterBehaviourMessage(const std::string& messageName, std::function<void()> func);

		// 매개변수 있는 함수 등록 (std::function 버전)
		template<typename... Args>
		void RegisterBehaviourParamMessage(const std::string& messageName, std::function<void(Args...)> func) {
			m_behaviourParamMessages.emplace(messageName, BehaviourParamMessageData(messageName, this, func));
		}

		void UnregisterBehaviourMessage(const std::string& messageName);

		void SetExecutionOrder(int order) { m_executionOrder = order; }

		virtual ~Behaviour();

	public:
		bool GetEnabled() const { return m_enabled; }
		void SetEnabled(bool value);

		bool IsActiveAndEnabled();
	};
}
