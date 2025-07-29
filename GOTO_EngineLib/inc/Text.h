#pragma once
#include "Graphic.h"
#include "Font.h"

namespace GOTOEngine
{
	class Text : public Graphic
	{
	private:
		Font* m_font = nullptr;
		void AdditionalInitialize() override;
	public:
		
		void SetFont(Font* font);
		void SetFont(Font* font, size_t size);

		void SetFont(const std::wstring filePath);
		void SetFont(const std::wstring filePath, size_t size);

		Font* GetFont() { return m_font; };

		TextHoriAlign horizontalAlign = TextHoriAlign::Left;
		TextVertAlign verticalAlign = TextVertAlign::Up;

		FontStyle fontStyle = FontStyle::Normal;

		std::wstring text;
		size_t size = 24;

		void Render() override;
	};
}