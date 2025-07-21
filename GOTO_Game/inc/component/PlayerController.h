#pragma once
#include <ScriptBehaviour.h>
#include <InputManager.h>
#include <TimeManager.h>
#include <Transform.h>
#include <Camera.h>

namespace GOTOEngine
{
	class PlayerController : public ScriptBehaviour
	{
	private:
		
	public:
		PlayerController()
		{
			SetExecutionOrder(10);
		}
		float moveSpeed = 245.0f;
		bool isSub = false;

		void Update()
		{
			float hInput = 0.0f;
			float vInput = 0.0f;

			if (isSub)
			{
				hInput = (INPUT_GET_KEY(KeyCode::RightArrow) ? 1.0f : 0.0f) + (INPUT_GET_KEY(KeyCode::LeftArrow) ? -1.0f : 0.0f);
				vInput = (INPUT_GET_KEY(KeyCode::UpArrow) ? 1.0f : 0.0f) + (INPUT_GET_KEY(KeyCode::DownArrow) ? -1.0f : 0.0f);
			}
			else
			{
				hInput = (INPUT_GET_KEY(KeyCode::D) ? 1.0f : 0.0f) + (INPUT_GET_KEY(KeyCode::A) ? -1.0f : 0.0f);
				vInput = (INPUT_GET_KEY(KeyCode::W) ? 1.0f : 0.0f) + (INPUT_GET_KEY(KeyCode::S) ? -1.0f : 0.0f);
			}

			if (INPUT_GET_KEYDOWN(KeyCode::F))
				SCENE_CHANGE_SCENE(L"SampleScene");

			GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition() + (Vector2{ hInput, vInput }.Normalized() * moveSpeed * m_sizefactor * TIME_GET_DELTATIME()));
		}
	};
}