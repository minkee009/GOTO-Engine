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
		GameObject* otherGameObject = nullptr; // 다른 게임 오브젝트를 참조할 수 있는 변수
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
			// 초기화 코드 작성
			if (Object::IsValidObject(otherGameObject))
			{
				otherGameObject->SetActive(false);
			}
			std::wcout << "TestComponent Awake called for GameObject: " << GetGameObject()->name << std::endl;
		}
		void Start()
		{
			// 시작 시 실행할 코드 작성

			std::wcout << "TestComponent Start called for GameObject: " << GetGameObject()->name << std::endl;
		}
		void OnEnable()
		{
			std::wcout << "TestComponent OnEnable called for GameObject: " << GetGameObject()->name << std::endl;
		}
		
		void OnDisable()
		{
			// 시작 시 실행할 코드 작성

			std::wcout << "TestComponent OnDisable called for GameObject: " << GetGameObject()->name << std::endl;
		}

		void FixedUpdate()
		{
			std::wcout << "TestComponent FixedUpdate called for GameObject: " << GetGameObject()->name << std::endl;
		}

		void Update()
		{
			// 매 프레임마다 실행할 코드 작성
			m_timer += TimeManager::Get()->GetDeltaTime();

			if (m_timer > 3.0f && Object::IsValidObject(otherGameObject) && !otherGameObject->IsActiveSelf())
			{
				otherGameObject->SetActive(true);
			}
		}
		void OnDestroy()
		{
			// 오브젝트 파괴 시 실행할 코드 작성
			std::wcout << "TestComponent OnDestroy called for GameObject: " << GetGameObject()->name << std::endl;
		}

		void OnCollisionEnter(int num)
		{
			std::wcout << "TestComponent OnCollisionEnter called for GameObject: " << GetGameObject()->name << "/ num : " << num << std::endl;
		}
	};
}