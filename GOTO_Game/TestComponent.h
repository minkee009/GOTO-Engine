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
			REGISTER_BEHAVIOUR_MESSAGE(Awake);
			REGISTER_BEHAVIOUR_MESSAGE(Start);
			REGISTER_BEHAVIOUR_MESSAGE(OnEnable);
			REGISTER_BEHAVIOUR_MESSAGE(OnDisable);
			REGISTER_BEHAVIOUR_MESSAGE(FixedUpdate);
			REGISTER_BEHAVIOUR_MESSAGE(Update);
			REGISTER_BEHAVIOUR_MESSAGE(OnDestroy);
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

			std::wcout << "TestComponent OnDisable called for GameObject: " << (Object::IsValidObject(GetGameObject()) ? GetGameObject()->name : L"ss") << std::endl;
		}

		void FixedUpdate()
		{
			// 고정 업데이트 시 실행할 코드 작성
			// 이 예제에서는 사용하지 않지만, 필요시 구현할 수 있습니다.
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
			std::wcout << "TestComponent OnDestroy called for GameObject: " << (Object::IsValidObject(GetGameObject()) ? GetGameObject()->name : L"ss") << std::endl;
		}

		void OnCollisionEnter(GameObject* other)
		{
		}
	};
}