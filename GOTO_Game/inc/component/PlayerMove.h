#pragma once
#include <ScriptBehaviour.h>

namespace GOTOEngine
{
	class PlayerMove : public ScriptBehaviour
	{
		void Awake(){}
	public:
    PlayerMove()
    {
        REGISTER_BEHAVIOUR_METHOD(Awake);
    }
	};
}