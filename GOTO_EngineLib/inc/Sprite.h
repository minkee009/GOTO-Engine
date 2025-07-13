#pragma once
#include "Resource.h"

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
		
		float m_width = 0;
		float m_height = 0;
		float m_pivotX = 0.5f;
		float m_pivotY = 0.5f;

		void Dispose() override;

		void LoadFromFilePath(const std::wstring& filePath) override;
	public:
		bool IsValidRawData() override;
		float GetWidth() const { return m_width; }
		float GetHeight() const { return m_height; }
		void SetWidth(float value) { m_width = value; }
		void SetHeight(float value) { m_height = value; }
		float GetPivotX() const { return m_pivotX; }
		float GetPivotY() const { return m_pivotY; }
		void SetPivotX(float value) { m_pivotX = value; }
		void SetPivotY(float value) { m_pivotY = value; }
	};
}