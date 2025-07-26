#pragma once
#include "Behaviour.h"
#include "Vector2.h"

namespace GOTOEngine
{
	class AudioListener : public Behaviour
	{
	private:
		Vector2 m_lastPosition;
		bool m_positionDirty;

		void OnEnable();
		void OnDisable();
		void OnDestroy();

	protected:
		~AudioListener();

	public:
		AudioListener();
		void UpdateListenerPosition();

		static AudioListener* GetMain();
		void SetAsMainListener();

		void RegisterMessages();
	};
}