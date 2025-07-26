#include "AudioListener.h"
#include "AudioManager.h"
#include "Transform.h"
#include "GameObject.h"
#include <iostream>

using namespace GOTOEngine;

AudioListener::AudioListener()
	: m_lastPosition(Vector2{ 0, 0 })
	, m_positionDirty(true)
{
	RegisterMessages();
}

AudioListener::~AudioListener()
{
}

void AudioListener::RegisterMessages()
{
	REGISTER_BEHAVIOUR_MESSAGE(OnEnable);
	REGISTER_BEHAVIOUR_MESSAGE(OnDisable);
	REGISTER_BEHAVIOUR_MESSAGE(OnDestroy);
}

void AudioListener::OnEnable()
{
	if (AudioManager::Get()->IsInitialized())
	{
		AudioManager::Get()->RegisterListener(this);
#ifdef _DEBUG
		std::cout << "AudioListener enabled." << std::endl;
#endif
	}
}

void AudioListener::OnDisable()
{
	if (AudioManager::Get()->IsInitialized())
	{
		AudioManager::Get()->UnRegisterListener(this);
#ifdef _DEBUG
		std::cout << "AudioListener disabled." << std::endl;
#endif
	}
}

void AudioListener::OnDestroy()
{
	OnDisable();
}

void AudioListener::UpdateListenerPosition()
{
	if (!AudioManager::Get()->IsInitialized() || !GetTransform())
		return;

	Vector2 currentPosition = GetTransform()->GetPosition();

	if (currentPosition.x != m_lastPosition.x ||
		currentPosition.y != m_lastPosition.y ||
		m_positionDirty)
	{
		ma_engine* engine = AudioManager::Get()->GetEngine();

		// ������ ��ġ ���� (Z���� 0)
		ma_engine_listener_set_position(engine, 0, currentPosition.x, currentPosition.y, 0.0f);

		m_lastPosition = currentPosition;
		m_positionDirty = false;

#ifdef _DEBUG_VERBOSE
		std::cout << "Listener position: (" << currentPosition.x << ", " << currentPosition.y << ")" << std::endl;
#endif
	}
}

AudioListener* AudioListener::GetMain()
{
	return AudioManager::Get()->GetMainListener();
}

void AudioListener::SetAsMainListener()
{
	AudioManager::Get()->m_mainListener = this;
}