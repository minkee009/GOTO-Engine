#include "MonsterAttack.h"
#include "InputManager.h"

void GOTOEngine::MonsterAttack::Update()
{
	if (INPUT_GET_KEYDOWN(KeyCode::Space))
	{
		onAttack.Invoke(10);
	}
}
