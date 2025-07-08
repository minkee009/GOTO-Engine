#include "Behaviour.h"
#include "BehaviourManager.h"

GOTOEngine::Behaviour::Behaviour() : Component()
{
	m_enabled = true;
	BehaviourManager::Get()->RegisterBehaviour(this); // BehaviourManager에 등록
}

GOTOEngine::Behaviour::~Behaviour()
{
	BehaviourManager::Get()->UnregisterBehaviour(this); // BehaviourManager에서 제거
	m_messages.clear();
}

void GOTOEngine::Behaviour::SetEnabled(bool value)
{
	if (value != m_enabled)
	{
		//바뀔 때 무언갈 실행하는 코드 작성하기
		m_enabled = value;
	}
}

bool GOTOEngine::Behaviour::IsActiveAndEnabled()
{
	return m_enabled && Object::IsValidObject(GetGameObject()) && GetGameObject()->IsActiveInHierarchy();
}
