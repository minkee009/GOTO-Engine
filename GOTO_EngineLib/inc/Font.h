#pragma once
#include "Resource.h"
#include "RenderManager.h"
#include "IRenderFont.h"
#include <unordered_map>

namespace GOTOEngine
{
	enum class FontStyle
	{
		Normal,
		Bold,
		Italic,
		BoldItalic
	};

	class FontStyleHelper
	{
		static IRenderFontStyle ToRenderFontStyle(FontStyle style)
		{
			switch (style)
			{
			case FontStyle::Normal:
				return IRenderFontStyle::Normal;
			case FontStyle::Bold:
				return IRenderFontStyle::Bold;
			case FontStyle::Italic:
				return IRenderFontStyle::Italic;
			case FontStyle::BoldItalic:
				return IRenderFontStyle::BoldItalic;
			}
		}
	};

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