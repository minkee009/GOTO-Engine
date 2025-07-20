#pragma once
#include "Behaviour.h"
#include "AnimatorController.h"

namespace GOTOEngine
{
	class Animator : public Behaviour
	{
	private:
		AnimatorController* m_controller;
	public:
		Animator()
		{

		}
	};
}