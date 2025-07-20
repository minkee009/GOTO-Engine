#pragma once
#include "Resource.h"
#include "RenderManager.h"
#include "Sprite.h"
#include <vector>

namespace GOTOEngine
{
	struct AnimationKeyframe
	{
		std::wstring spriteName;
		float time;
	};
	
	class AnimaitonClip : public Resource
	{
	private:
		friend class ResourceManager;
		void LoadFromFilePath(const std::wstring& filePath) override;
		bool IsValidData() override { return m_keyframes.size() > 0; }
		std::vector<AnimationKeyframe*> m_keyframes;
		bool m_isLoop;
		float m_duration;
		std::wstring m_texturePath;
		~AnimaitonClip();
	public:
		const bool& IsLoop() const { return m_isLoop; }
		const std::vector<AnimationKeyframe*>& GetKeyframes() const { return m_keyframes; }
		const float& GetDuration() const { return m_duration; }
	};
}