#include "AnimationClip.h"

void GOTOEngine::AnimaitonClip::LoadFromFilePath(const std::wstring& filePath)
{
}

GOTOEngine::AnimaitonClip::~AnimaitonClip()
{
	for (auto frame : m_keyframes)
	{
		delete frame;
	}
	m_keyframes.clear();
}
