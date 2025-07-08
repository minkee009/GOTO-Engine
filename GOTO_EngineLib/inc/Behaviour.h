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

		// �⺻ ������
		BehaviourParamMessageData() = default;

		// ������ ����: std::function�� ���� ���� Ÿ���� �����Ͽ� ���
		template<typename... Args>
		BehaviourParamMessageData(const std::string& name, Behaviour* owner, std::function<void(Args...)> funcPtr)
			: owner(owner), messageName(name)
		{
			parameterTypes = { std::type_index(typeid(Args))... };

			// ���ٷ� Ÿ�� ��ȯ ó��
			// CallWithParameters�� Args... ���� ���� �����Ͽ� �Ű����� Ÿ���� �˸��ϴ�.
			func = [funcPtr](const std::vector<std::any>& params) {
				if (params.size() != sizeof...(Args)) {
					throw std::runtime_error("Parameter count mismatch");
				}
				// CallWithParameters ȣ�� �� Args... ���� ��������� ����
				CallWithParameters<Args...>(funcPtr, params, std::index_sequence_for<Args...>{});
				};
		}

	private:
		// CallWithParameters�� ���ø� ���ڸ� Func�� �Բ� Args...�� ���� �޽��ϴ�.
		template<typename... OriginalArgs, typename Func, std::size_t... I>
		static void CallWithParameters(Func&& funcPtr, const std::vector<std::any>& params, std::index_sequence<I...>) {
			// ���� OriginalArgs...�� ���� �Լ� �ñ״�ó�� Ÿ�� ���̹Ƿ�
			// std::tuple<OriginalArgs...>�� ���� ����� �� �ֽ��ϴ�.
			using ArgsTuple = std::tuple<OriginalArgs...>;

			funcPtr(std::any_cast<std::tuple_element_t<I, ArgsTuple>>(params[I])...);
		}
	};


	//���� ��� ����޴� ������Ʈ�Դϴ�.
	class Behaviour : public Component
	{
	private:
		friend class BehaviourManager;
		friend class GameObject;

		std::unordered_map <std::string, BehaviourMessageData> m_behaviourMessages; // �Լ� �̸��� �Լ� �����͸� �����ϴ� ����
		std::unordered_map <std::string, BehaviourParamMessageData> m_behaviourParamMessages; // �Լ� �̸��� �Լ� �����͸� �����ϴ� ����

		void CallBehaviourMessage(const std::string& messageName);
		void CallBehaviourMessage(const std::string& messageName, const std::vector<std::any>& params);

	protected:
		Behaviour();

		bool m_enabled;
		int m_executionOrder = 0; // ���� ������ ��Ÿ���� ����

		// �Ű����� ���� �Լ� ���
		void RegisterBehaviourMessage(const std::string& messageName, std::function<void()> func);

		// �Ű����� �ִ� �Լ� ��� (std::function ����)
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
