#pragma once
#include "Graphic.h"
#include "Sprite.h"

namespace GOTOEngine
{
	class Image : public Graphic
	{
	private:
		Sprite* m_sprite = nullptr;
	public:
		Image() : Graphic() {}
		void SetSprite(Sprite* sprite);
		void SetSprite(Sprite* sprite, Rect srcRect);

		void SetSprite(const std::wstring filePath);
		void SetSprite(const std::wstring filePath, Rect srcRect);

		Sprite* GetSprite() { return m_sprite; };
		void Render() override;
	};
}