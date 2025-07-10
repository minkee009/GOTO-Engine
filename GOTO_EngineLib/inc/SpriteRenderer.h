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
		void Render(Matrix4x4& matrix) override;
	};
}
