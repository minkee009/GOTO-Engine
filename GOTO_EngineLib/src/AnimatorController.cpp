#include "AnimatorController.h"

void GOTOEngine::AnimatorController::LoadFromFilePath(const std::wstring& filePath)
{
}

GOTOEngine::AnimatorState* GOTOEngine::AnimatorController::GetState(std::string name)
{
	auto it = m_states.find(name);
	if (it != m_states.end())
		return m_states[name];
	else
		return nullptr;
}