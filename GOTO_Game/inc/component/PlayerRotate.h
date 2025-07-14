#pragma once
#include <ScriptBehaviour.h>
#include <TimeManager.h>
#include <Transform.h>

namespace GOTOEngine
{
	class PlayerRotate : public ScriptBehaviour
	{
	private:
		float m_rot = 0;
	public:
    PlayerRotate()
    {
        REGISTER_BEHAVIOUR_MESSAGE(Update);
    }
		float rotateSpeed = 45.0f;

		void Update()
		{
		    m_rot += rotateSpeed * TIME_GET_DELTATIME();
			GetTransform()->SetLocalRotation(m_rot);
		}
	};
}