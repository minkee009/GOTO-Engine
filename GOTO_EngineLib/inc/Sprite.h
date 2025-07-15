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

		Sprite();
		~Sprite();
		
		float m_pivotX = 0.5f;
		float m_pivotY = 0.5f;
		Rect m_rect;

		void Dispose() override;

		void LoadFromFilePath(const std::wstring& filePath) override;
	public:
		bool IsValidRawData() override;

		const Rect& GetRect() const { return m_rect; }
		Rect SetRect(const Rect& rect) { m_rect = rect; }

		float GetPivotX() const { return m_pivotX; }
		float GetPivotY() const { return m_pivotY; }
		void SetPivotX(float value) { m_pivotX = value; }
		void SetPivotY(float value) { m_pivotY = value; }
	};
}