#pragma once
#include "Renderer.h"
#include "Rect.h"
#include "Font.h"
#include "TextHelper.h"

namespace GOTOEngine
{
	//======== 스프라이트 렌더러 ========
	//
	//          * 유의 사항 *
	// 
	//  GetFont()를 대입식에 집어넣은 경우
	//  반드시 IncreaseRefCount()를 호출하여 
	//  참조카운트가 누락되지 않도록 합니다.
	// 
	//  ex) Font* renderFont = textRendererComp->GetFont();
	//      renderFont.IncreaseRefCount();
	//
	// 
	//  이후 사용을 끝마치는 경우 반드시 
	//  DecreaseRefCount()를 호출하여 마찬가지로
	//  참조카운트가 누락되지 않도록 합니다.
	// 
	//  ex) renderFont.DecreaseRefCount(); //코드 블럭 끝에
	// 
	//
	//=================================

	class TextRenderer : public Renderer
	{
	private:
		Font* m_font;
		Vector2 m_pivot = { 0.5f, 0.5f };

		void Render(Matrix3x3& matrix);
	public:
		std::wstring text;
		size_t size;
		FontStyle fontStyle;
		Rect rect;
		Color color;
		TextHoriAlign horizontalAlign;
		TextVertAlign verticalAlign;

		void Dispose() override { if (m_font) { m_font->DecreaseRefCount(); m_font = nullptr; } }
		Font* GetFont() const { return m_font; }
		void SetFont(Font* font);
	};
}