#include "Behaviour.h"

void GOTOEngine::Behaviour::CallBehaviourMessage(const std::string& messageName, bool checkFirstCall)
{
	auto it = m_behaviourMessages.find(messageName);
	if (it != m_behaviourMessages.end())
	{
		if (checkFirstCall && it->second.isFirstCall)
		{
			it->second.isFirstCall = false; // ù ȣ�� ���Ŀ��� false�� ����
		}
		else if (checkFirstCall && !it->second.isFirstCall)
		{
			return; // ù ȣ���� �ƴϸ� �Լ� �������� ����
		}
		it->second.func();
	}
}

void GOTOEngine::Behaviour::RegisterBehaviourMessage(const std::string& messageName, std::function<void()> func)
{
	BehaviourMessageData data;
	data.owner = this;
	data.messageName = messageName;
	data.func = func;
	m_behaviourMessages.insert({ messageName, data });
}

void GOTOEngine::Behaviour::UnregisterBehaviourMessage(const std::string& messageName)
{
	auto it = m_behaviourMessages.find(messageName);
	if (it != m_behaviourMessages.end())
	{
		m_behaviourMessages.erase(it);
	}
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
	return m_enabled && GetGameObject()->IsActiveInHierarchy(); // Object::IsValidObject(GetGameObject()) &&;
}
