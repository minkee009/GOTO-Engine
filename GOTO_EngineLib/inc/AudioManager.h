#pragma once
#include "Singleton.h"
#include <vector>
#include <miniaudio.h>

namespace GOTOEngine
{
	class AudioListener;
	class AudioSource;

	class AudioManager : public Singleton<AudioManager>
	{
	private:
		friend class Engine;
		friend class AudioListener;
		friend class AudioSource;

		ma_engine m_audioEngine;
		bool m_isInitialized;

		std::vector<AudioListener*> m_listeners;
		std::vector<AudioSource*> m_audioSources;
		AudioListener* m_mainListener;

		void StartUp();
		void ShutDown();
		void Update();

		void RegisterListener(AudioListener* listener);
		void UnRegisterListener(AudioListener* listener);
		void RegisterAudioSource(AudioSource* source);
		void UnRegisterAudioSource(AudioSource* source);

	public:
		ma_engine* GetEngine() { return &m_audioEngine; }
		bool IsInitialized() const { return m_isInitialized; }
		AudioListener* GetMainListener() { return m_mainListener; }

		void SetMasterVolume(float volume);
		float GetMasterVolume() const;
	};
}