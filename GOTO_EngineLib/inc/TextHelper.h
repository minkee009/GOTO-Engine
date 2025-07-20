#pragma once
#include "IRenderFont.h"

namespace GOTOEngine
{
	enum class TextHoriAlign
	{
		Right = -1,
		Center = 0,
		Left = 1
	};

	enum class TextVertAlign
	{
		Down = -1,
		Center = 0,
		Up = 1
	};

	enum class FontStyle
	{
		Normal,
		Bold,
		Italic,
		BoldItalic
	};

	class FontStyleHelper
	{
	public:
		static IRenderFontStyle ToRenderFontStyle(FontStyle style)
		{
			switch (style)
			{
			case FontStyle::Normal:
				return IRenderFontStyle::Normal;
			case FontStyle::Bold:
				return IRenderFontStyle::Bold;
			case FontStyle::Italic:
				return IRenderFontStyle::Italic;
			case FontStyle::BoldItalic:
				return IRenderFontStyle::BoldItalic;
			default:
				return IRenderFontStyle::Normal;
			}
		}
	};
}