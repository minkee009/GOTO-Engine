#pragma once
#include "Behaviour.h"

namespace GOTOEngine 
{
    // 매크로 정의
#define REGISTER_BEHAVIOUR_METHOD(func) RegisterMessage(#func, &std::remove_reference_t<decltype(*this)>::func);
	class ScriptBehaviour : public Behaviour
	{
	public:
		ScriptBehaviour() = default;
		virtual ~ScriptBehaviour() = default;
	};
}