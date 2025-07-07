#pragma once
#include <ScriptBehaviour.h>
#include <iostream>

namespace GOTOEngine
{
	class TestComponent : public ScriptBehaviour
	{
	public:
		TestComponent()
		{
			SetExecutionOrder(12);

			//OnDestroy ���
			RegisterBehaviourMessage("OnDestroy", [this]() { OnDestroy(); });
		}
		virtual ~TestComponent() = default;
		void Awake() override
		{
			// �ʱ�ȭ �ڵ� �ۼ�
		}
		void Start() override
		{
			// ���� �� ������ �ڵ� �ۼ�
		}
		void Update() override
		{
			// �� �����Ӹ��� ������ �ڵ� �ۼ�
		}
		void OnDestroy() override
		{
			// ������Ʈ �ı� �� ������ �ڵ� �ۼ�
			std::cout << "TestComponent OnDestroy called for GameObject: " << std::endl;
		}
	};
}