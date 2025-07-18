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