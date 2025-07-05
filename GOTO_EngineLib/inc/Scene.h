#pragma once
#include <vector>
#include <string>
#include "Delegate.h"

namespace GOTOEngine
{
	class Scene
	{
	private:
		friend class GameObject;

		void RegisterGameObject(GameObject* go);
		void UnregisterGameObject(GameObject* go);
	protected:
		const std::wstring m_sceneName;
		std::vector<GameObject*> m_gameObjects;
	public:
		Scene(const std::wstring sceneName) : m_sceneName(sceneName) {}
		virtual ~Scene() { Clear(); }

		//������ �ڵ�--------------------//
		virtual void Update() {}		//
		virtual void FixedUpdate() {}	//
		//------------------------------//

		void Clear();
		void Reset() { Exit(); Enter(); }

		virtual void Enter() { }
		virtual void Exit() { Clear(); }
		const std::wstring& GetSceneName() const { return m_sceneName; }
	};
}
