#pragma once
#include <ScriptBehaviour.h>
#include <InputManager.h>
#include <RenderManager.h>
#include <TimeManager.h>
#include <Transform.h>
#include <Camera.h>
#include <Animator.h>

namespace GOTOEngine
{
	class PlayerFSMController : public ScriptBehaviour
	{
	private:
		Animator* m_animator = nullptr;
		SpriteRenderer* m_renderer = nullptr;
		bool m_timeSlower = false;
	public:
    PlayerFSMController()
    {
        REGISTER_BEHAVIOUR_MESSAGE(Awake);
        REGISTER_BEHAVIOUR_MESSAGE(OnGUI);
        REGISTER_BEHAVIOUR_MESSAGE(Update);
    }
		float moveSpeed = 145.0f;

		void Awake()
		{
			m_animator = GetComponent<Animator>();
			m_renderer = GetComponent<SpriteRenderer>();
		}

		void Update()
		{
			float hInput = (INPUT_GET_KEY(KeyCode::RightArrow) ? 1.0f : 0.0f) + (INPUT_GET_KEY(KeyCode::LeftArrow) ? -1.0f : 0.0f);
			float vInput = (INPUT_GET_KEY(KeyCode::UpArrow) ? 1.0f : 0.0f) + (INPUT_GET_KEY(KeyCode::DownArrow) ? -1.0f : 0.0f);

			if (hInput || vInput)
				m_animator->SetBool(L"Move", true);
			else
				m_animator->SetBool(L"Move", false);

			bool attack = INPUT_GET_KEYDOWN(KeyCode::Space);

			if (attack)
				m_animator->SetTrigger(L"Attack");

			if (m_animator->GetCurrentStateName() != L"anim_knight_attack")
			{
				if (IsValidObject(m_renderer))
					m_renderer->SetFlipX(hInput != 0 ? (hInput < 0) : m_renderer->GetFlipX());

				GetTransform()->SetPosition(GetTransform()->GetPosition() + (Vector2{ hInput, vInput }.Normalized() * moveSpeed * TIME_GET_DELTATIME()));
			}

			if (INPUT_GET_KEYDOWN(KeyCode::T))
			{
				m_timeSlower = !m_timeSlower;
				TimeManager::Get()->SetTimeScale(m_timeSlower ? 0.25f : 1.0f);
			}
		}

		void OnGUI()
		{
			RenderManager::Get()->DrawString(L"방향키 - 이동 \n Space바 - 공격 \n T키 - 시간 변경");
		}
	};
}