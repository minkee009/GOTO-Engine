#pragma once
#include "Resource.h"
#include "RenderManager.h"
#include "IRenderFont.h"
#include "TextHelper.h"
#include <unordered_map>

namespace GOTOEngine
{
	class Font : public Resource
	{
	private:
		friend class ResourceManager;
		void LoadFromFilePath(const std::wstring& filePath) override;
		bool IsValidRawData() override { return m_fontHandle; }
		IRenderFont* m_fontHandle;
		~Font();
	public:
		//static Font* CreateFontFromOSFont(const std::wstring& fontName, size_t size = 16);
		IRenderFont* GetFont() { return m_fontHandle; }
	};
}