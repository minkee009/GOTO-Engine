#include "Behaviour.h"
#include "BehaviourManager.h"

void GOTOEngine::Behaviour::CallBehaviourMessage(const std::string& messageName)
{
	auto it = m_behaviourMessages.find(messageName);
	if (it != m_behaviourMessages.end())
	{
		it->second.func();
	}
}

void GOTOEngine::Behaviour::CallBehaviourMessage(const std::string& messageName, const std::vector<std::any>& params)
{
	auto it = m_behaviourParamMessages.find(messageName);
	if (it != m_behaviourParamMessages.end())
	{
		it->second.func(params);
	}
}

GOTOEngine::Behaviour::Behaviour() : Component()
{
	m_enabled = true;
	BehaviourManager::Get()->RegisterBehaviour(this); // BehaviourManager에 등록
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
	auto noParam = m_behaviourMessages.find(messageName);
	if (noParam != m_behaviourMessages.end())
	{
		m_behaviourMessages.erase(noParam);
		return;
	}
	auto withParam = m_behaviourParamMessages.find(messageName);
	if (withParam != m_behaviourParamMessages.end())
	{
		m_behaviourParamMessages.erase(withParam);
	}
}
GOTOEngine::Behaviour::~Behaviour()
{
	BehaviourManager::Get()->UnregisterBehaviour(this); // BehaviourManager에서 제거
	m_behaviourMessages.clear();
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
