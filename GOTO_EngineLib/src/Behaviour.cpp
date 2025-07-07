#include "Behaviour.h"

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
	return m_enabled && GetGameObject()->IsActiveInHierarchy(); // Object::IsValidObject(GetGameObject()) &&;
}
