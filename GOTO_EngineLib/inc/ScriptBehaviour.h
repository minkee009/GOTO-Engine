#pragma once
#include "Behaviour.h"

namespace GOTOEngine 
{
    // 매크로 정의
	class ScriptBehaviour : public Behaviour
	{
	public:
		ScriptBehaviour() = default;
		virtual ~ScriptBehaviour() = default;
	};
}