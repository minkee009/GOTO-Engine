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

void SceneManager::Update()
{
	if (m_nextScene)
	{
		if (m_currentScene)
			m_currentScene->Exit();
		m_currentScene = m_nextScene;
		m_nextScene = nullptr;

		m_currentScene->Enter();
	}

	//������ �ڵ�
	m_currentScene->Update();
}
 
//������ �ڵ�
void GOTOEngine::SceneManager::FixedUpdate()
{
	m_currentScene->FixedUpdate();
}


