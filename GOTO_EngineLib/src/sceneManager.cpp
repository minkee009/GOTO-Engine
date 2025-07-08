#include "SceneManager.h"

using namespace GOTOEngine;

void SceneManager::StartUp()
{
	m_currentScene = nullptr;
	m_nextScene = nullptr;
}


void SceneManager::ShutDown()
{
	for (auto& pair : m_sceneMap)
	{
		if (pair.second)
			delete pair.second;
	}
	m_sceneMap.clear();
	m_currentScene = nullptr;
	m_nextScene = nullptr;
}

bool SceneManager::CheckSceneChange()
{
	if (m_nextScene)
	{
		if (m_currentScene)
			m_currentScene->Exit();

		m_currentScene = m_nextScene;
		m_nextScene = nullptr;

		m_currentScene->Enter();
		return true;
	}

	return false;
}


