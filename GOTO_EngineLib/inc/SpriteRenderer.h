#pragma once
#include "Renderer.h"
#include "Sprite.h"

namespace GOTOEngine
{
	class SpriteRenderer : public Renderer
	{
	private:
		friend class RenderManager;
		Sprite* m_sprite;
		void Render(Matrix3x3& matrix) override;
	public:
		SpriteRenderer(): m_sprite(nullptr) {}
		void SetSprite(Sprite* sprite);
		void Dispose() override { if (m_sprite) { m_sprite->DecreaseRefCount(); m_sprite = nullptr; } }
		const Sprite* GetSprite() const { return m_sprite; } //���� ī��Ʈ ���� ����
	};
 }
