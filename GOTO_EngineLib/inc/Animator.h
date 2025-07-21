#pragma once
#include "Behaviour.h"
#include "AnimatorController.h"
#include "RuntimeAnimatorController.h"
#include "SpriteRenderer.h"

namespace GOTOEngine
{
	class Animator : public Behaviour
	{
	private:
		AnimatorController* m_controller; //메타데이터
		RuntimeAnimatorController* m_runtimeController; //fsm, 생성 및 파괴 책임은 오로지 Animator에게만
		SpriteRenderer* m_spriteRenderer;
	public:
		Animator();

		AnimatorController* GetAnimatorController() { return m_controller; } //참조 카운트 꼭 하기
		void SetAnimatorController(AnimatorController* controller);

		RuntimeAnimatorController* GetRuntimeAnimatorController() { return m_runtimeController; }

		bool GetTrigger(std::wstring name) { if (IsValidObject(m_runtimeController) && !m_runtimeController->IsDestroyed()) return m_runtimeController->GetTrigger(name); return false; }
		bool GetBool(std::wstring name) { if (IsValidObject(m_runtimeController) && !m_runtimeController->IsDestroyed()) return m_runtimeController->GetBool(name); return false; }
		int GetInt(std::wstring name) { if (IsValidObject(m_runtimeController) && !m_runtimeController->IsDestroyed()) return m_runtimeController->GetInt(name); return 0; }
		float GetFloat(std::wstring name) { if (IsValidObject(m_runtimeController) && !m_runtimeController->IsDestroyed()) return m_runtimeController->GetFloat(name); return 0.0f; }

		void SetTrigger(std::wstring name) { if (IsValidObject(m_runtimeController) && !m_runtimeController->IsDestroyed()) m_runtimeController->SetTrigger(name); }
		void ResetTrigger(std::wstring name) { if (IsValidObject(m_runtimeController) && !m_runtimeController->IsDestroyed()) m_runtimeController->ResetTrigger(name); }
		void SetBool(std::wstring name, bool value) { if (IsValidObject(m_runtimeController) && !m_runtimeController->IsDestroyed()) m_runtimeController->SetBool(name, value); }
		void SetInt(std::wstring name, int value) { if (IsValidObject(m_runtimeController) && !m_runtimeController->IsDestroyed()) m_runtimeController->SetInt(name, value); }
		void SetFloat(std::wstring name, float value) { if (IsValidObject(m_runtimeController) && !m_runtimeController->IsDestroyed()) m_runtimeController->SetFloat(name, value); }
	};
}