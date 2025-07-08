#include "Behaviour.h"
#include "BehaviourManager.h"

GOTOEngine::Behaviour::Behaviour() : Component()
{
	m_enabled = true;
	BehaviourManager::Get()->RegisterBehaviour(this); // BehaviourManager�� ���
}

GOTOEngine::Behaviour::~Behaviour()
{
	BehaviourManager::Get()->UnregisterBehaviour(this); // BehaviourManager���� ����
	m_messages.clear();
}

void GOTOEngine::Behaviour::SetEnabled(bool value)
{
	if (value != m_enabled)
	{
		//�ٲ� �� ���� �����ϴ� �ڵ� �ۼ��ϱ�
		m_enabled = value;
	}
}

bool GOTOEngine::Behaviour::IsActiveAndEnabled()
{
	return m_enabled && Object::IsValidObject(GetGameObject()) && GetGameObject()->IsActiveInHierarchy();
}
