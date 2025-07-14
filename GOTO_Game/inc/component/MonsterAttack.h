#pragma once
#include <ScriptBehaviour.h>
#include <Delegate.h>

namespace GOTOEngine
{
	class MonsterAttack : public ScriptBehaviour
	{
	private:
		void Update();
	public:
    MonsterAttack()
    {
        REGISTER_BEHAVIOUR_MESSAGE(Update);
    }
		Delegate<void,int> onAttack;
	};
}