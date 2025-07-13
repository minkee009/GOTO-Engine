#pragma once
#include <ScriptBehaviour.h>
#include <TimeManager.h>
#include <Transform.h>

namespace GOTOEngine
{
	class PlayerRotate : public ScriptBehaviour
	{
		float m_rot = 0;
	public:
    PlayerRotate()
    {
        REGISTER_BEHAVIOUR_MESSAGE(Update);
    }
		void Update()
		{
			m_rot += 45.0f * TIME_GET_DELTATIME();
			GetTransform()->SetLocalRotation(m_rot);
		}
	};
}