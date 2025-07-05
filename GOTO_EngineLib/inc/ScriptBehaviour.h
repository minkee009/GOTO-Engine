#pragma once
#include "Behaviour.h"

namespace GOTOEngine 
{
	class ScriptBehaviour : public Behaviour
	{
	public:
		ScriptBehaviour() = default;
		virtual ~ScriptBehaviour() = default;
		// 스크립트 초기화
		virtual void Start() {}
		// 매 프레임 업데이트
		virtual void Update(float deltaTime) {}
		// 고정 업데이트 (물리 연산 등)
		virtual void FixedUpdate(float fixedDeltaTime) {}
		// 렌더링 전 호출
		virtual void PreRender() {}
		// 렌더링 후 호출
		virtual void PostRender() {}
		// 스크립트 종료
		virtual void OnDestroy() {}
	};
}