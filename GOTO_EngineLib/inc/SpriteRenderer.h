#pragma once
#include "Renderer.h"
#include "Sprite.h"
#include "Rect.h"

namespace GOTOEngine
{
	//======== ��������Ʈ ������ ========
	//
	//          * ���� ���� *
	// 
	//  GetSprite()�� ���ԽĿ� ������� ���
	//  �ݵ�� IncreaseRefCount()�� ȣ���Ͽ� 
	//  ����ī��Ʈ�� �������� �ʵ��� �մϴ�.
	// 
	//  ex) Sprite* renderSprite = spriteRendererComp->GetSprite();
	//      renderSprite.IncreaseRefCount();
	//
	// 
	//  ���� ����� ����ġ�� ��� �ݵ�� 
	//  DecreaseRefCount()�� ȣ���Ͽ� ����������
	//  ����ī��Ʈ�� �������� �ʵ��� �մϴ�.
	// 
	//  ex) renderSprite.DecreaseRefCount(); //�ڵ� �� ����
	// 
	//
	//=================================

	class SpriteRenderer : public Renderer
	{
	private:
		friend class RenderManager;
		Sprite* m_sprite;
		bool m_flipX;
		bool m_flipY;
		void Render(Matrix3x3& viewMatrix) override;
	public:
		SpriteRenderer(): m_sprite(nullptr), m_flipX(false), m_flipY(false) {}
		void SetSprite(Sprite* sprite);
		void SetSprite(Sprite* sprite, Rect srcRect);

		bool GetFlipX() const { return m_flipX; }
		bool GetFlipY() const { return m_flipY; }
		void SetFlipX(bool value) { m_flipX = value; }
		void SetFlipY(bool value) { m_flipY = value; }

		void Dispose() override { if (IsValidObject(m_sprite) && !m_sprite->IsDestroyed()) { m_sprite->DecreaseRefCount(); m_sprite = nullptr; } }
		Sprite* GetSprite() { return m_sprite; } //���� ī��Ʈ ��������� �����ϱ�
	};
 }
