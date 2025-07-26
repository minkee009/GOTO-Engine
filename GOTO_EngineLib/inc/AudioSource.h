#pragma once
#include "Behaviour.h"
#include "Vector2.h"
#include <miniaudio.h>

namespace GOTOEngine
{
	class AudioClip;

	//======== ����� �ҽ� ========
	//
	//          * ���� ���� *
	// 
	//  GetClip()�� ���ԽĿ� �־���� ���
	//  �ݵ�� IncreaseRefCount()�� ȣ���Ͽ� 
	//  ����ī��Ʈ�� �þ�� �ʵ��� �մϴ�.
	// 
	//  ex) AudioClip* clip = audioSource->GetClip();
	//      clip->IncreaseRefCount();
	//
	// 
	//  ���� ����� ����ĥ ��� �ݵ�� 
	//  DecreaseRefCount()�� ȣ���Ͽ� ����������
	//  ����ī��Ʈ�� �þ�� �ʵ��� �մϴ�.
	// 
	//  ex) clip->DecreaseRefCount(); //�ڵ� ��� ����
	// 
	//
	//=================================

	class AudioSource : public Behaviour
	{
	private:
		AudioClip* m_clip;
		ma_sound m_sound;
		bool m_soundInitialized;

		float m_volume;
		float m_pitch;
		bool m_loop;
		bool m_playOnAwake;
		bool m_spatialAudio;

		float m_minDistance;
		float m_maxDistance;

		bool m_isPlaying;
		bool m_isPaused;
		Vector2 m_lastPosition;
		bool m_positionDirty;

		void OnEnable();
		void OnDisable();
		void OnDestroy();

		void InitializeSound();
		void CleanupSound();
		void ApplySettings();

	protected:
		~AudioSource();

	public:
		void SetClip(AudioClip* clip);
		AudioClip* GetClip() const { return m_clip; } // ���� ī��Ʈ ��������� �����ϱ�
		AudioSource();

		void Play();
		void Stop();
		void Pause();
		void Resume();

		bool IsPlaying() const { return m_isPlaying && !m_isPaused; }
		bool IsPaused() const { return m_isPaused; }

		void SetVolume(float volume);
		float GetVolume() const { return m_volume; }

		void SetPitch(float pitch);
		float GetPitch() const { return m_pitch; }

		void SetLoop(bool loop);
		bool IsLooping() const { return m_loop; }

		void SetPlayOnAwake(bool playOnAwake) { m_playOnAwake = playOnAwake; }
		bool GetPlayOnAwake() const { return m_playOnAwake; }

		void SetSpatialAudio(bool enabled);
		bool IsSpatialAudio() const { return m_spatialAudio; }

		void SetMinDistance(float distance);
		float GetMinDistance() const { return m_minDistance; }

		void SetMaxDistance(float distance);
		float GetMaxDistance() const { return m_maxDistance; }

		void UpdateTransform();

		void RegisterMessages();
	};
}