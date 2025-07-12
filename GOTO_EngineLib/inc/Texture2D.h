#pragma once
#include "Resource.h"
#include "RenderManager.h"
#include "IRenderBitmap.h"

namespace GOTOEngine
{
	class Texture2D : public Resource
	{
	private:
		friend class ResourceManager;
		IRenderBitmap* m_bitmapHandle;
		void LoadFromFilePath(const std::wstring& filePath) override;
	public:
		Texture2D();
		~Texture2D();
		const IRenderBitmap* GetBitmap() const { return m_bitmapHandle; }
		float GetWidth() { return m_bitmapHandle ? m_bitmapHandle->GetWidth() : 0; }
		float GetHeight() { return  m_bitmapHandle ? m_bitmapHandle->GetHeight() : 0; }
		bool IsValidRawData() override;
	};
}