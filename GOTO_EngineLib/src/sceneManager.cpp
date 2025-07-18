#include "SceneManager.h"

using namespace GOTOEngine;

void SceneManager::StartUp()
{
	m_currentScene = nullptr;
	m_nextScene = nullptr;
	m_dontDestroyOnLoadScene = new Scene(L"DontDestroyOnLoad");
}


void SceneManager::ShutDown()
{
	for (auto& pair : m_sceneMap)
	{
		if (pair.second)
			delete pair.second;
	}
	m_sceneMap.clear();
	delete m_dontDestroyOnLoadScene;
	m_currentScene = nullptr;
	m_nextScene = nullptr;
	m_dontDestroyOnLoadScene = nullptr;
}

bool SceneManager::CheckSceneChange()
{
	if (m_nextScene)
	{
		if (m_currentScene)
			m_currentScene->Clear();

		m_currentScene = m_nextScene;
		m_nextScene = nullptr;

		m_currentScene->Initialize();
		return true;
	}

	return false;
}


