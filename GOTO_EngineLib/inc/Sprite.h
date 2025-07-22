#pragma once
#include "Resource.h"
#include "Rect.h"

namespace GOTOEngine
{
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

		void Dispose() override;

		void LoadFromFilePath(const std::wstring& filePath) override;
	public:
		Sprite();
		Sprite(Texture2D* texture);
		bool IsValidData() override;

		const Rect& GetRect() const { return m_rect; }
		void SetRect(const Rect& rect) { m_rect = rect; }

		void SetTexture(Texture2D* texture);
		Texture2D* GetTexture() { return m_texture; } //���� ī��Ʈ ��������� �����ϱ�

		float GetPivotX() const { return m_pivotX; }
		float GetPivotY() const { return m_pivotY; }
		void SetPivotX(float value) { m_pivotX = value; }
		void SetPivotY(float value) { m_pivotY = value; }
	};
}