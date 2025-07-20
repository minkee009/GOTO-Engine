#include "AnimationClip.h"
#include <fstream>
#include <json.hpp>
#include <iostream>
#include "WStringHelper.h"

using json = nlohmann::json;

void GOTOEngine::AnimaitonClip::LoadFromFilePath(const std::wstring& filePath)
{
	std::ifstream inFile(filePath);
	if (inFile.is_open())
	{
		json jFromFile;
		inFile >> jFromFile;
		inFile.close();
		name = STR_TO_WSTR(jFromFile["clipName"]);
		m_texturePath = STR_TO_WSTR(jFromFile["texturePath"]);
		m_isLoop = jFromFile["loop"];
		m_duration = jFromFile["duration"];
		for (auto keyframe : jFromFile["frames"])
			m_keyframes.emplace_back(new AnimationKeyframe{ STR_TO_WSTR(keyframe["sprite"]),keyframe["time"] });
	}
}

GOTOEngine::AnimaitonClip::~AnimaitonClip()
{
	for (auto frame : m_keyframes)
	{
		delete frame;
	}
	m_keyframes.clear();
}
