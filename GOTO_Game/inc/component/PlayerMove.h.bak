#pragma once
#include <ScriptBehaviour.h>
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

	void Update()
	{
		GetTransform()->SetPosition(GetTransform()->GetPosition() + (Vector2::Right() * 45.0f * TIME_GET_DELTATIME()));
	}
	};
}