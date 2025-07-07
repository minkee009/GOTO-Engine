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

			//OnDestroy 등록
			RegisterBehaviourMessage("OnDestroy", [this]() { OnDestroy(); });
		}
		virtual ~TestComponent() = default;
		void Awake() override
		{
			// 초기화 코드 작성
		}
		void Start() override
		{
			// 시작 시 실행할 코드 작성
		}
		void Update() override
		{
			// 매 프레임마다 실행할 코드 작성
		}
		void OnDestroy() override
		{
			// 오브젝트 파괴 시 실행할 코드 작성
			std::cout << "TestComponent OnDestroy called for GameObject: " << std::endl;
		}
	};
}