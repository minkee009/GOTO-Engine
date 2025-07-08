#pragma once
#include "Component.h"
#include "GameObject.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <any>


namespace GOTOEngine
{
	// ���� ����
	class Behaviour;

	// === �޽��� ���̽� ===
	struct BehaviourMessageBase
	{
		virtual ~BehaviourMessageBase() = default;
		virtual void InvokeRaw(void** args) = 0;
		virtual void InvokeVoid() = 0;
	};

	template<typename T>
	struct BehaviourMessage : BehaviourMessageBase
	{
		using FuncType = void(T::*)();

		Behaviour* owner;
		FuncType func;

		BehaviourMessage(Behaviour* owner, FuncType func)
			: owner(owner), func(func) {
		}

		void InvokeVoid() override
		{
			(static_cast<T*>(owner)->*func)();
		}

		void InvokeRaw(void**) override
		{
			return;
			//throw std::runtime_error("This message does not accept parameters.");
		}
	};

	template<typename T, typename... Args>
	struct BehaviourParamMessage : BehaviourMessageBase
	{
		using FuncType = void(T::*)(Args...);

		Behaviour* owner;
		FuncType func;

		BehaviourParamMessage(Behaviour* owner, FuncType func)
			: owner(owner), func(func) {
		}

		void InvokeRaw(void** args) override
		{
			InvokeImpl(args, std::index_sequence_for<Args...>{});
		}

		void InvokeVoid() override
		{
			return;
			//throw std::runtime_error("This message requires parameters.");
		}

	private:
		template<std::size_t... I>
		void InvokeImpl(void** args, std::index_sequence<I...>)
		{
			(static_cast<T*>(owner)->*func)(*reinterpret_cast<typename std::tuple_element<I, std::tuple<Args...>>::type*>(args[I])...);
		}
	};


	//���� ��� ����޴� ������Ʈ�Դϴ�.
	class Behaviour : public Component
	{
	private:
		friend class BehaviourManager;
		friend class GameObject;

		std::unordered_map<std::string, std::unique_ptr<BehaviourMessageBase>> m_messages;


		// ȣ�� - �Ű����� ����
		void CallMessage(const std::string& name)
		{
			auto it = m_messages.find(name);
			if (it == m_messages.end())
				return;
			it->second->InvokeVoid();
		}

		// ȣ�� - �Ű����� ���� (void* �迭�� ����)
		template<typename... Args>
		void CallMessage(const std::string& name, Args&&... args)
		{
			void* argArray[] = { (void*)&args... };

			auto it = m_messages.find(name);
			if (it == m_messages.end())
				return;
			it->second->InvokeRaw(argArray);
		}

	protected:
		Behaviour();

		bool m_enabled;
		int m_executionOrder = 0; // ���� ������ ��Ÿ���� ����

		template<typename T>
		void RegisterMessage(const std::string& name, void(T::* func)())
		{
			m_messages[name] = std::make_unique<BehaviourMessage<T>>(this, func);
		}

		template<typename T, typename... Args>
		void RegisterMessage(const std::string& name, void(T::* func)(Args...))
		{
			m_messages[name] = std::make_unique<BehaviourParamMessage<T, Args...>>(this, func);
		}

		void SetExecutionOrder(int order) { m_executionOrder = order; }

		virtual ~Behaviour();

	public:
		bool GetEnabled() const { return m_enabled; }
		void SetEnabled(bool value);

		bool IsActiveAndEnabled();
	};
}
