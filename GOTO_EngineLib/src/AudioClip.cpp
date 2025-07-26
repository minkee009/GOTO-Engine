#include "AudioClip.h"
#include "AudioManager.h"
#include "WStringHelper.h"
#include <iostream>

using namespace GOTOEngine;

AudioClip::AudioClip()
	: m_loadMode(AudioLoadMode::DecompressOnLoad)
	, m_length(0.0f)
	, m_channels(0)
	, m_sampleRate(0)
	, m_isLoaded(false)
{
}

AudioClip::~AudioClip()
{
}

void AudioClip::LoadFromFilePath(const std::wstring& filePath)
{
	if (!AudioManager::Get()->IsInitialized())
	{
#ifdef _DEBUG
		std::cout << "AudioManager not initialized." << std::endl;
#endif
		return;
	}

	// 파일 확장자로 로드 모드 결정
	m_loadMode = DetermineLoadMode(filePath);

	// 파일 정보 읽기
	std::string filePathUtf8 = WStringHelper::wstring_to_string(filePath);

	ma_decoder_config config = ma_decoder_config_init_default();
	ma_decoder decoder;

	ma_result result = ma_decoder_init_file(filePathUtf8.c_str(), &config, &decoder);
	if (result != MA_SUCCESS)
	{
		m_isLoaded = false;
		return;
	}

	m_channels = decoder.outputChannels;
	m_sampleRate = decoder.outputSampleRate;

	// 길이 계산
	ma_uint64 frameCount;
	if (ma_decoder_get_length_in_pcm_frames(&decoder, &frameCount) == MA_SUCCESS)
	{
		m_length = (float)frameCount / (float)m_sampleRate;
	}

	ma_decoder_uninit(&decoder);
	m_isLoaded = true;

#ifdef _DEBUG
	std::string modeStr = (m_loadMode == AudioLoadMode::DecompressOnLoad) ? "Memory" : "Stream";
	std::cout << "AudioClip loaded: " << filePathUtf8
		<< " (Mode: " << modeStr << ", Length: " << m_length << "s)" << std::endl;
#endif
}

AudioLoadMode AudioClip::DetermineLoadMode(const std::wstring& filePath)
{
	// 확장자 추출
	size_t lastDot = filePath.find_last_of(L".");
	if (lastDot == std::wstring::npos)
		return AudioLoadMode::DecompressOnLoad;

	std::wstring extension = filePath.substr(lastDot + 1);

	// 소문자 변환
	for (auto& c : extension)
		c = towlower(c);

	// 긴 파일들은 스트리밍, 짧은 파일들은 메모리 로드
	if (extension == L"mp3" || extension == L"ogg" || extension == L"flac")
		return AudioLoadMode::Stream;
	else
		return AudioLoadMode::DecompressOnLoad;
}

ma_result AudioClip::CreateSound(ma_sound* sound, ma_engine* engine) const
{
	if (!m_isLoaded || !sound || !engine)
		return MA_INVALID_ARGS;

	std::string filePathUtf8 = WStringHelper::wstring_to_string(m_filePath);

	ma_uint32 flags = MA_SOUND_FLAG_NO_PITCH | MA_SOUND_FLAG_NO_SPATIALIZATION;

	if (m_loadMode == AudioLoadMode::Stream)
		flags |= MA_SOUND_FLAG_STREAM;

	return ma_sound_init_from_file(engine, filePathUtf8.c_str(), flags, nullptr, nullptr, sound);
}