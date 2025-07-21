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
	class PlayerController : public ScriptBehaviour
	{
	private:
		Animator* m_animator = nullptr;
	public:
    PlayerController()
    {
        REGISTER_BEHAVIOUR_MESSAGE(Awake);
        REGISTER_BEHAVIOUR_MESSAGE(OnGUI);
        REGISTER_BEHAVIOUR_MESSAGE(Update);
    }
		float moveSpeed = 245.0f;
		bool isSub = false;

		void Awake()
		{
			m_animator = GetComponent<Animator>();
		}

		void Update()
		{
			if (INPUT_GET_KEYDOWN(KeyCode::Alpha1))
			{
				if (IsValidObject(m_animator))
					m_animator->Play(L"anim_knight_run");
			}
			if (INPUT_GET_KEYDOWN(KeyCode::Alpha2))
			{
				if (IsValidObject(m_animator))
					m_animator->Play(L"anim_knight_jump");
			}
			if (INPUT_GET_KEYDOWN(KeyCode::Alpha3))
			{
				if (IsValidObject(m_animator))
					m_animator->Play(L"anim_knight_fall");
			}
		}

		void OnGUI()
		{
			RenderManager::Get()->DrawString(L"숫자 1키 - 달리기 \n 숫자 2키 - 점프 \n 숫자 3키 - 낙하");
		}
	};
}