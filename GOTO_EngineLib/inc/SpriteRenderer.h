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
		void Render(Matrix3x3& matrix) override;
	public:
		SpriteRenderer(): m_sprite(nullptr) {}
		void SetSprite(Sprite* sprite);
		void SetSprite(Sprite* sprite, Rect srcRect);
		void Dispose() override { if (m_sprite) { m_sprite->DecreaseRefCount(); m_sprite = nullptr; } }
		Sprite* GetSprite() { return m_sprite; } //���� ī��Ʈ ��������� �����ϱ�
	};
 }
