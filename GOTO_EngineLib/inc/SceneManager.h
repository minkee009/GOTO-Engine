#pragma once
#include <string>
#include <unordered_map>
#include "Scene.h"
#include "Singleton.h"

namespace GOTOEngine
{
	class SceneManager : public Singleton<SceneManager>
	{
	public:
		void ChangeScene(const std::wstring& sceneName)
		{
			m_nextScene = FindScene(sceneName);
		}

		template<typename T>
		void AddScene(const std::wstring& sceneName)
		{
			static_assert(std::is_base_of<Scene, T>::value, "T는 Scene을 상속받는 타입이어야 합니다.");
			
			auto it = m_sceneMap.find(sceneName);
			if (it != m_sceneMap.end())
				return;

			Scene* scene = new T(sceneName);

			m_sceneMap[sceneName] = scene;
		}

		Scene* FindScene(const std::wstring& sceneName)
		{
			auto it = m_sceneMap.find(sceneName);
			return it != m_sceneMap.end() ? it->second : nullptr;
		}

		Scene* GetCurrentScene()
		{
			return m_currentScene;
		}

	private:
		friend class Engine;
		Scene* m_currentScene;
		Scene* m_nextScene;
		std::unordered_map<std::wstring, Scene*> m_sceneMap;


		void StartUp();
		void ShutDown();

		void Update();
	};
}

#define SCENE_ADD_SCENE GOTOEngine::SceneManager::Get()->AddScene
#define SCENE_CHANGE_SCENE GOTOEngine::SceneManager::Get()->ChangeScene
#define SCENE_GET_CURRENTSCENE GOTOEngine::SceneManager::Get()->GetCurrentScene