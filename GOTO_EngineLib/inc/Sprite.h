#pragma once
#include "Resource.h"
#include "Rect.h"

namespace GOTOEngine
{
	enum class SpriteRenderMode
	{
		Point,         // 생 비트맵: 보간 없음, 날 픽셀 (Nearest)
		Bilinear,      // 뭉게짐: 2D 보간 (기본)
		//Trilinear,     // 트리보간: mipmap 포함 (3D용 주로)
		//Crisp,         // 뚜렷한데 고해상도 대응 (추가 옵션)
		//Custom         // 커스텀 셰이더 등 사용자 지정 모드
	};

	class Texture2D;
	class Sprite : public Resource
	{
	private:
		friend class SpriteRenderer;
		friend class ResourceManager;
		Texture2D* m_texture;

		~Sprite();
		
		float m_pivotX;
		float m_pivotY;
		Rect m_rect;

		bool m_flipX;
		bool m_flipY;

		SpriteRenderMode m_renderMode;

		void Dispose() override;

		void LoadFromFilePath(const std::wstring& filePath) override;
	public:
		Sprite();
		Sprite(Texture2D* texture);
		bool IsValidData() override;

		const Rect& GetRect() const { return m_rect; }
		void SetRect(const Rect& rect) { m_rect = rect; }

		void SetTexture(Texture2D* texture);
		Texture2D* GetTexture() { return m_texture; } //참조 카운트 명시적으로 설정하기

		void SetRenderMode(SpriteRenderMode mode) { m_renderMode = mode; }
		const SpriteRenderMode& GetRenderMode() { return m_renderMode; }

		float GetPivotX() const { return m_pivotX; }
		float GetPivotY() const { return m_pivotY; }
		bool GetFlipX() const { return m_flipX; }
		bool GetFlipY() const { return m_flipY; }
		void SetFlipX(bool value) { m_flipX = value; }
		void SetFlipY(bool value) { m_flipY = value; }
		void SetPivotX(float value) { m_pivotX = value; }
		void SetPivotY(float value) { m_pivotY = value; }
	};
}