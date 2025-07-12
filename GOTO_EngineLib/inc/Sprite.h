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

		void LoadFromFilePath(const std::wstring& filePath) override;
	public:
		bool IsValidRawData() override;
	};
}