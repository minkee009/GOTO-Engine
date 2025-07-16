#pragma once
#include "Resource.h"
#include "IRenderFont.h"
#include <unordered_map>

namespace GOTOEngine
{
	class Font : public Resource
	{
	private:
		friend class ResourceManager;
		void LoadFromFilePath(const std::wstring& filePath) override;
		bool IsValidRawData() override;
		std::unordered_map<size_t, IRenderFont*> m_fontHandles;
		size_t m_size;
	public:
		static Font* CreateFontFromOSFont(const std::wstring& fontName);
	};
}