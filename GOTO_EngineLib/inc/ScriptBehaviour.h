#pragma once
#include "Behaviour.h"

namespace GOTOEngine 
{
    // ��ũ�� ����
#define REGISTER_BEHAVIOUR_MESSAGE(methodName) \
    RegisterBehaviourMessage(#methodName, [this]() { methodName(); })
	class ScriptBehaviour : public Behaviour
	{
	public:
		ScriptBehaviour() = default;
		virtual ~ScriptBehaviour() = default;
	};
}