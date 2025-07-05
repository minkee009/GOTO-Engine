#pragma once
#include <Singleton.h>
#include <unordered_map>
#include <string>
#include <queue>
#include "Behaviour.h"

namespace GOTOEngine
{
	class BehaviourManager : public Singleton<BehaviourManager>
	{
	public:
		void StartUp();
		void ShutDown();
	private:
		friend class Engine;
		void Update();
		void LateUpdate();
		void FixedUpdate();

		std::vector<Behaviour*> m_behaviours;
		std::unordered_map<std::string, Behaviour*> m_activatedBehaviours;
		std::queue<Behaviour*> m_createdBehaviors;
	};
}