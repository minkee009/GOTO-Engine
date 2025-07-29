#pragma once
#include "Renderer.h"
#include "Rect.h"
#include "Font.h"
#include "TextHelper.h"

namespace GOTOEngine
{
	//======== ��������Ʈ ������ ========
	//
	//          * ���� ���� *
	// 
	//  GetFont()�� ���ԽĿ� ������� ���
	//  �ݵ�� IncreaseRefCount()�� ȣ���Ͽ� 
	//  ����ī��Ʈ�� �������� �ʵ��� �մϴ�.
	// 
	//  ex) Font* renderFont = textRendererComp->GetFont();
	//      renderFont.IncreaseRefCount();
	//
	// 
	//  ���� ����� ����ġ�� ��� �ݵ�� 
	//  DecreaseRefCount()�� ȣ���Ͽ� ����������
	//  ����ī��Ʈ�� �������� �ʵ��� �մϴ�.
	// 
	//  ex) renderFont.DecreaseRefCount(); //�ڵ� �� ����
	// 
	//
	//=================================

	class TextRenderer : public Renderer
	{
	private:
		Font* m_font;

		void Render(Matrix3x3& matrix);
	public:
		TextRenderer() 
			: m_font(nullptr)
			, size(16)
			, fontStyle(FontStyle::Normal)
			, rect({0,0,100,100})
			, pivot({ 0.5f, 0.5f })
			, color({255,255,255,255}) 
			, horizontalAlign(TextHoriAlign::Center)
			, verticalAlign(TextVertAlign::Center)
		{
		}


		std::wstring text;
		size_t size;
		FontStyle fontStyle;
		Rect rect;
		Vector2 pivot;
		Color color;
		TextHoriAlign horizontalAlign;
		TextVertAlign verticalAlign;

		void Dispose() override { if (IsValidObject(m_font) && !m_font->IsDestroyed()) { m_font->DecreaseRefCount(); m_font = nullptr; } }
		Font* GetFont() const { return m_font; }
		void SetFont(Font* font);
		void SetFont(std::wstring filePath);
	};
}