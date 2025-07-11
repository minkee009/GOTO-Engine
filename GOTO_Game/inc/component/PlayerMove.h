#pragma once
#include <ScriptBehaviour.h>

namespace GOTOEngine
{
	class PlayerMove : public ScriptBehaviour
	{
		void OnSceneLoad(const Scene* scene, int layer) {}
	public:
    PlayerMove()
    {
        SetExecutionOrder(10);
        REGISTER_BEHAVIOUR_MESSAGE(OnSceneLoad);
    }
	};
}