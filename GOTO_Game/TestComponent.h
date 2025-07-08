#pragma once
#include <ScriptBehaviour.h>
#include <iostream>

namespace GOTOEngine
{
	class TestComponent : public ScriptBehaviour
	{
	private:
		float m_timer = 0.0f;
	public:
		GameObject* otherGameObject = nullptr; // �ٸ� ���� ������Ʈ�� ������ �� �ִ� ����
		virtual ~TestComponent() = default;
		TestComponent()
		{
			REGISTER_BEHAVIOUR_METHOD(Awake);
			REGISTER_BEHAVIOUR_METHOD(OnEnable);
			RegisterMessage("Start", &TestComponent::Start);
			RegisterMessage("FixedUpdate", &TestComponent::FixedUpdate);
			RegisterMessage("Update", &TestComponent::Update);
			RegisterMessage("OnDisable", &TestComponent::OnDisable);
			RegisterMessage("OnDestroy", &TestComponent::OnDestroy);
			RegisterMessage("OnCollisionEnter", &TestComponent::OnCollisionEnter);
		}

		void Awake()
		{
			// �ʱ�ȭ �ڵ� �ۼ�
			if (Object::IsValidObject(otherGameObject))
			{
				otherGameObject->SetActive(false);
			}
			std::wcout << "TestComponent Awake called for GameObject: " << GetGameObject()->name << std::endl;
		}
		void Start()
		{
			// ���� �� ������ �ڵ� �ۼ�

			std::wcout << "TestComponent Start called for GameObject: " << GetGameObject()->name << std::endl;
		}
		void OnEnable()
		{
			std::wcout << "TestComponent OnEnable called for GameObject: " << GetGameObject()->name << std::endl;
		}
		
		void OnDisable()
		{
			// ���� �� ������ �ڵ� �ۼ�

			std::wcout << "TestComponent OnDisable called for GameObject: " << GetGameObject()->name << std::endl;
		}

		void FixedUpdate()
		{
			std::wcout << "TestComponent FixedUpdate called for GameObject: " << GetGameObject()->name << std::endl;
		}

		void Update()
		{
			// �� �����Ӹ��� ������ �ڵ� �ۼ�
			m_timer += TimeManager::Get()->GetDeltaTime();

			if (m_timer > 3.0f && Object::IsValidObject(otherGameObject) && !otherGameObject->IsActiveSelf())
			{
				otherGameObject->SetActive(true);
			}
		}
		void OnDestroy()
		{
			// ������Ʈ �ı� �� ������ �ڵ� �ۼ�
			std::wcout << "TestComponent OnDestroy called for GameObject: " << GetGameObject()->name << std::endl;
		}

		void OnCollisionEnter(int num)
		{
			std::wcout << "TestComponent OnCollisionEnter called for GameObject: " << GetGameObject()->name << "/ num : " << num << std::endl;
		}
	};
}