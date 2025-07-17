#pragma once

namespace GOTOEngine
{
	enum IRenderFontStyle
	{
		Normal = 0,
		Bold = 1,
		Italic = 2,
		BoldItalic = 3
	};

	class IRenderFont
	{
	public:
		virtual ~IRenderFont() = default; 
	};
}

