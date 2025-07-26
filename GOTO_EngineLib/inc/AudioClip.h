#pragma once
#include "Resource.h"
#include <miniaudio.h>

namespace GOTOEngine
{
	enum class AudioLoadMode
	{
		DecompressOnLoad,  // 메모리에 전체 로드 (효과음용)
		Stream            // 스트리밍 (배경음악용)
	};

	class AudioClip : public Resource
	{
	private:
		friend class ResourceManager;
		friend class AudioSource;

		AudioLoadMode m_loadMode;
		float m_length;
		ma_uint32 m_channels;
		ma_uint32 m_sampleRate;
		bool m_isLoaded;

		void LoadFromFilePath(const std::wstring& filePath) override;
		AudioLoadMode DetermineLoadMode(const std::wstring& filePath);

	public:
		AudioClip();
		~AudioClip();

		bool IsValidData() override { return m_isLoaded; }
		float GetLength() const { return m_length; }
		ma_uint32 GetChannels() const { return m_channels; }
		ma_uint32 GetSampleRate() const { return m_sampleRate; }
		AudioLoadMode GetLoadMode() const { return m_loadMode; }

		ma_result CreateSound(ma_sound* sound, ma_engine* engine) const;
	};
}