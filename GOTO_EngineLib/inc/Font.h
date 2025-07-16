#pragma once
#include "Resource.h"
#include "IRenderFontGlyph.h"
#include "RenderManager.h"
#include <unordered_map>

namespace GOTOEngine
{
	class Font : public Resource
	{
	private:
		friend class ResourceManager;
		void LoadFromFilePath(const std::wstring& filePath) override;
		bool IsValidRawData() override;
		std::unordered_map<size_t, IRenderFontGlyph*> m_fontHandles;
		size_t m_size;
	public:
		static Font* CreateFontFromOSFont(const std::wstring& fontName);
		IRenderFontGlyph* GetFont() 
		{
			if (m_fontHandles.find(m_size) == m_fontHandles.end())
			{
				//RenderManager::Get()->>
			}
			
			return m_fontHandles[m_size];
		}
	};
}