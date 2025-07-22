#pragma once
#include "Resource.h"
#include "RenderManager.h"
#include "IRenderBitmap.h"
#include "Rect.h"

namespace GOTOEngine
{
	enum class TextureRenderMode
	{
		Point,         // 생 비트맵: 보간 없음, 날 픽셀 (Nearest)
		Bilinear,      // 뭉게짐: 2D 보간 (기본)
		//Trilinear,     // 트리보간: mipmap 포함 (3D용 주로)
		//Crisp,         // 뚜렷한데 고해상도 대응 (추가 옵션)
		//Custom         // 커스텀 셰이더 등 사용자 지정 모드
	};

	class Texture2D : public Resource
	{
	private:
		friend class ResourceManager;
		IRenderBitmap* m_bitmapHandle;
		Rect m_rect;
		TextureRenderMode m_renderMode;
		void LoadFromFilePath(const std::wstring& filePath) override;
		~Texture2D();
	public:
		Texture2D();

		void SetRenderMode(TextureRenderMode mode) { m_renderMode = mode; }
		const TextureRenderMode& GetRenderMode() { return m_renderMode; }

		const IRenderBitmap* GetBitmap() const { return m_bitmapHandle; }
		const Rect& GetRect() const { return m_rect; }
		bool IsValidData() override { return m_bitmapHandle; }
	};
}