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
		AnimatorController* m_controller; //��Ÿ������
		RuntimeAnimatorController* m_runtimeController; //fsm, ���� �� �ı� å���� ������ Animator���Ը�
		SpriteRenderer* m_spriteRenderer;

		void Dispose() override;
	public:
		Animator() : m_controller(nullptr), m_runtimeController(nullptr), m_spriteRenderer(nullptr)
		{
			REGISTER_BEHAVIOUR_MESSAGE(OnAnimationUpdate);
		}

		void OnAnimationUpdate();

		AnimatorController* GetAnimatorController() { return m_controller; } //���� ī��Ʈ �� �ϱ�
		void SetAnimatorController(AnimatorController* controller);

		const RuntimeAnimatorController* GetRuntimeAnimatorController() const { return m_runtimeController; }

		void Play(std::wstring stateName);

		const std::wstring& GetCurrentStateName() const { return m_runtimeController->GetCurrentStateName(); }

		bool GetTrigger(std::wstring name) const { if (IsValidObject(m_runtimeController) && !m_runtimeController->IsDestroyed()) return m_runtimeController->GetTrigger(name); return false; }
		bool GetBool(std::wstring name) const { if (IsValidObject(m_runtimeController) && !m_runtimeController->IsDestroyed()) return m_runtimeController->GetBool(name); return false; }
		int GetInt(std::wstring name) const { if (IsValidObject(m_runtimeController) && !m_runtimeController->IsDestroyed()) return m_runtimeController->GetInt(name); return 0; }
		float GetFloat(std::wstring name) const { if (IsValidObject(m_runtimeController) && !m_runtimeController->IsDestroyed()) return m_runtimeController->GetFloat(name); return 0.0f; }

		void SetTrigger(std::wstring name) { if (IsValidObject(m_runtimeController) && !m_runtimeController->IsDestroyed()) m_runtimeController->SetTrigger(name); }
		void ResetTrigger(std::wstring name) { if (IsValidObject(m_runtimeController) && !m_runtimeController->IsDestroyed()) m_runtimeController->ResetTrigger(name); }
		void SetBool(std::wstring name, bool value) { if (IsValidObject(m_runtimeController) && !m_runtimeController->IsDestroyed()) m_runtimeController->SetBool(name, value); }
		void SetInt(std::wstring name, int value) { if (IsValidObject(m_runtimeController) && !m_runtimeController->IsDestroyed()) m_runtimeController->SetInt(name, value); }
		void SetFloat(std::wstring name, float value) { if (IsValidObject(m_runtimeController) && !m_runtimeController->IsDestroyed()) m_runtimeController->SetFloat(name, value); }
	};
}