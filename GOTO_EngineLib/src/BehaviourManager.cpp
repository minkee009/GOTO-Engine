#include "BehaviourManager.h"

void GOTOEngine::BehaviourManager::BroadCastBehaviourMessage(const std::string& funcName)
{
	for (auto& behaviour : m_activeBehaviours)
	{
		behaviour->CallBehaviourMessage(funcName);
	}
}

void GOTOEngine::BehaviourManager::StartUp()
{
}

void GOTOEngine::BehaviourManager::ShutDown()
{
}


