#pragma once
#include "Renderer.h"
#include "Sprite.h"

namespace GOTOEngine
{
	class SpriteRenderer : Renderer
	{
	private:
		friend class RenderManager;
		Sprite* m_sprite;
		void Render(Matrix3x3& matrix) override;
	public:
		void SetSprite(Sprite* sprite);
		const Sprite* GetSprite() const { return m_sprite; } //참조 카운트 증가 없음
	};
 }
