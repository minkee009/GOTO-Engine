#pragma once
#include "Renderer.h"
#include "Rect.h"
#include "Font.h"

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
		size_t m_size;
		FontStyle m_fontStyle;

		Rect m_rect;
		Vector2 m_pivot = { 0.5f, 0.5f };

		void Render(Matrix3x3& matrix);
	public:
		void Dispose() override { if (m_font) { m_font->DecreaseRefCount(); m_font = nullptr; } }
		Font* GetFont() const { return m_font; }
		void SetFont(Font* font);
	};
}