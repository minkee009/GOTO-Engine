#pragma once
#include <ScriptBehaviour.h>

namespace GOTOEngine
{
	class MonsterAttack;
	class PlayerHealth : public ScriptBehaviour
	{
	private:
		int m_hp = 100;
		MonsterAttack* m_monAtk = nullptr;

		void Awake();
		void TakeDamage(int value);
		void OnDestroy();
	public:
    PlayerHealth()
    {
        REGISTER_BEHAVIOUR_MESSAGE(Awake);
        REGISTER_BEHAVIOUR_MESSAGE(OnDestroy);
    }

	int GetHP() { return m_hp; }
	};
}