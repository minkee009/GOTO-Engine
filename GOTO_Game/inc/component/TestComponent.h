#pragma once
#include <ScriptBehaviour.h>
#include <TimeManager.h>
#include <iostream>

namespace GOTOEngine
{
	class TestComponent : public ScriptBehaviour
	{
	private:
		float m_timer = 0.0f;
	public:
    TestComponent()
    {
        REGISTER_BEHAVIOUR_MESSAGE(Awake);
        REGISTER_BEHAVIOUR_MESSAGE(Update);
    }

		GameObject* otherGameObject = nullptr; // 다른 게임 오브젝트를 참조할 수 있는 변수
		virtual ~TestComponent() = default;

		void Awake()
		{
			// 초기화 코드 작성
			if (IsValidObject(otherGameObject))
			{
				otherGameObject->SetActive(false);
				Destroy(otherGameObject, 15.0f);
			}
			std::wcout << "TestComponent Awake called for GameObject: " << GetGameObject()->name << std::endl;
		}

		void Update()
		{
			// 매 프레임마다 실행할 코드 작성
			m_timer += TimeManager::Get()->GetDeltaTime();

			if (m_timer > 3.0f && IsValidObject(otherGameObject) && !otherGameObject->IsActiveSelf())
			{
				otherGameObject->SetActive(true);
			}
		}
	};
}