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
			REGISTER_BEHAVIOUR_MESSAGE(Awake);
			REGISTER_BEHAVIOUR_MESSAGE(Start);
			REGISTER_BEHAVIOUR_MESSAGE(OnEnable);
			REGISTER_BEHAVIOUR_MESSAGE(FixedUpdate);
			REGISTER_BEHAVIOUR_MESSAGE(Update);
			REGISTER_BEHAVIOUR_MESSAGE(OnDestroy);
		}
		void Awake()
		{
			// �ʱ�ȭ �ڵ� �ۼ�
			if (Object::IsValidObject(otherGameObject))
			{
				otherGameObject->SetActive(false);
			}
			std::cout << "TestComponent Awake called for GameObject: " << std::endl;
		}
		void Start()
		{
			// ���� �� ������ �ڵ� �ۼ�

			std::cout << "TestComponent Start called for GameObject: " << std::endl;
		}
		void OnEnable()
		{
			// ���� �� ������ �ڵ� �ۼ�

			std::cout << "TestComponent OnEnable called for GameObject: " << std::endl;
		}

		void FixedUpdate()
		{
			// ���� ������Ʈ �� ������ �ڵ� �ۼ�
			// �� ���������� ������� ������, �ʿ�� ������ �� �ֽ��ϴ�.
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
			std::cout << "TestComponent OnDestroy called for GameObject: " << std::endl;
		}

		void OnCollisionEnter(GameObject* other)
		{
		}
	};
}