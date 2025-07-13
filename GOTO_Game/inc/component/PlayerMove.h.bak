#pragma once
#include <ScriptBehaviour.h>
#include <InputManager.h>
#include <TimeManager.h>
#include <Transform.h>

namespace GOTOEngine
{
	class PlayerMove : public ScriptBehaviour
	{
	public:
    PlayerMove()
    {
        SetExecutionOrder(10);
        REGISTER_BEHAVIOUR_MESSAGE(Update);
    }
		float moveSpeed = 145.0f;

	void Update()
	{
		float hInput = (INPUT_GET_KEY(KeyCode::RightArrow) ? 1.0f : 0.0f) + (INPUT_GET_KEY(KeyCode::LeftArrow) ? -1.0f : 0.0f);
		float vInput = (INPUT_GET_KEY(KeyCode::UpArrow) ? 1.0f : 0.0f) + (INPUT_GET_KEY(KeyCode::DownArrow) ? -1.0f : 0.0f);

		GetTransform()->SetPosition(GetTransform()->GetPosition() + (Vector2{ hInput, vInput }.Normalized() *moveSpeed * TIME_GET_DELTATIME()));
	}
	};
}