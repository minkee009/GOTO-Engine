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

		struct BehaviourData
		{
			Behaviour* behaviour;
			std::unordered_map<std::string,std::function<void()> > funcs;
		};

		std::vector<Behaviour*> m_behaviours;	
		std::queue<BehaviourData> m_createdBehaviors;
	};
}