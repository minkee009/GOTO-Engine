#pragma once
#include "Renderer.h"
#include "Sprite.h"
#include "Rect.h"

namespace GOTOEngine
{
	//======== 스프라이트 렌더러 ========
	//
	//          * 유의 사항 *
	// 
	//  GetSprite()를 대입식에 집어넣은 경우
	//  반드시 IncreaseRefCount()를 호출하여 
	//  참조카운트가 누락되지 않도록 합니다.
	// 
	//  ex) Sprite* renderSprite = spriteRendererComp->GetSprite();
	//      renderSprite.IncreaseRefCount();
	//
	// 
	//  이후 사용을 끝마치는 경우 반드시 
	//  DecreaseRefCount()를 호출하여 마찬가지로
	//  참조카운트가 누락되지 않도록 합니다.
	// 
	//  ex) renderSprite.DecreaseRefCount(); //코드 블럭 끝에
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
		Sprite* GetSprite() { return m_sprite; } //참조 카운트 명시적으로 설정하기
	};
 }
