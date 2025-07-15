#pragma once
#include "Resource.h"
#include "RenderManager.h"
#include "IRenderBitmap.h"
#include "Rect.h"

namespace GOTOEngine
{
	class Texture2D : public Resource
	{
	private:
		friend class ResourceManager;
		IRenderBitmap* m_bitmapHandle;
		Rect m_rect;
		void LoadFromFilePath(const std::wstring& filePath) override;
	public:
		Texture2D();
		~Texture2D();
		
		const IRenderBitmap* GetBitmap() const { return m_bitmapHandle; }
		const Rect& GetRect() const { return m_rect; }
		bool IsValidRawData() override;
	};
}