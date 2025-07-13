#pragma once
#include <ScriptBehaviour.h>
#include <InputManager.h>
#include <TimeManager.h>
#include <Transform.h>

namespace GOTOEngine
{
	class CameraMove : public ScriptBehaviour
	{
	public:
    CameraMove()
    {
        SetExecutionOrder(10);
        REGISTER_BEHAVIOUR_MESSAGE(Update);
    }
		float moveSpeed = 245.0f;

		void Update()
		{
			float hInput = (INPUT_GET_KEY(KeyCode::A) ? 1.0f : 0.0f) + (INPUT_GET_KEY(KeyCode::D) ? -1.0f : 0.0f);
			float vInput = (INPUT_GET_KEY(KeyCode::W) ? 1.0f : 0.0f) + (INPUT_GET_KEY(KeyCode::S) ? -1.0f : 0.0f);

			GetTransform()->SetPosition(GetTransform()->GetPosition() + (Vector2{ hInput, vInput }.Normalized() * moveSpeed * TIME_GET_DELTATIME()));
		}
	};
}