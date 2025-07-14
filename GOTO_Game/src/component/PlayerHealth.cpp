#include "PlayerHealth.h"
#include "MonsterAttack.h"

void GOTOEngine::PlayerHealth::Awake()
{
	m_monAtk = FindObjectByType<MonsterAttack>();
	if (IsValidObject(m_monAtk))
	{
		m_monAtk->onAttack.Add(this, &PlayerHealth::TakeDamage);
	}
}

void GOTOEngine::PlayerHealth::TakeDamage(int value)
{
	m_hp -= value;

	if (m_hp <= 0)
		Destroy(this->GetGameObject());
}

void GOTOEngine::PlayerHealth::OnDestroy()
{
	if (IsValidObject(m_monAtk))
	{
		m_monAtk->onAttack.Remove(this, &PlayerHealth::TakeDamage);
	}
}
