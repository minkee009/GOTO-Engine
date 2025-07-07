#include "Behaviour.h"

void GOTOEngine::Behaviour::CallBehaviourMessage(const std::string& messageName)
{
	auto it = m_behaviourMessages.find(messageName);
	if (it != m_behaviourMessages.end())
	{
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
		//바뀔 때 무언갈 실행하는 코드 작성하기
		m_enabled = value;
	}
}

bool GOTOEngine::Behaviour::IsActiveAndEnabled()
{
	return m_enabled && GetGameObject()->IsActiveInHierarchy(); // Object::IsValidObject(GetGameObject()) &&;
}
