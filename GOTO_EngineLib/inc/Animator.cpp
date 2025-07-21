#include "Animator.h"
#include "TimeManager.h"

void GOTOEngine::Animator::OnAnimationUpdate()
{
	if (m_runtimeController)
	{
		m_runtimeController->Update(TIME_GET_DELTATIME());

		if (m_spriteRenderer)
			m_spriteRenderer->SetSprite(m_runtimeController->GetCurrentSprite());
	}
}

void GOTOEngine::Animator::Dispose()
{
	if (IsValidObject(m_controller) 
		&& !m_controller->IsDestroyed()) 
	{ 
		m_controller->DecreaseRefCount(); 
		m_controller = nullptr; 
	}

	if (IsValidObject(m_runtimeController)
		&& !m_runtimeController->IsDestroyed())
	{
		DestroyImmediate(m_runtimeController);
		m_runtimeController = nullptr;
	}

	m_spriteRenderer = nullptr;
}
void GOTOEngine::Animator::SetAnimatorController(AnimatorController* controller)
{
	if (m_controller != controller)
	{
		if (controller)
		{
			if (m_runtimeController)
				DestroyImmediate(m_runtimeController);

			m_runtimeController = new RuntimeAnimatorController(controller);

			controller->IncreaseRefCount();
		}
			
		if (m_controller)
			m_controller->DecreaseRefCount();
	}
}

void GOTOEngine::Animator::Play(std::wstring stateName)
{
	if (IsValidObject(m_runtimeController)
		&& !m_runtimeController->IsDestroyed())
	{
		m_runtimeController->ForceChangeState(stateName);
	}
}
