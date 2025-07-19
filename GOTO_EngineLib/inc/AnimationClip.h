#pragma once
#include "Resource.h"
#include "RenderManager.h"
#include "Sprite.h"
#include <vector>

namespace GOTOEngine
{
	struct AmimationKeyframe
	{
		std::wstring spritePath;
		float time;
	};
	
	class AnimaitonClip : public Resource
	{
	private:
		friend class ResourceManager;
		void LoadFromFilePath(const std::wstring& filePath) override;
		bool IsValidRawData() override { return m_keyframes.size() > 0; }
		std::vector<AmimationKeyframe*> m_keyframes;
		~AnimaitonClip();
	public:
		const std::vector<AmimationKeyframe*>& GetKeyframes() const { return m_keyframes; }
	};
}