#include "D2DFont.h"
#include "DWriteHelper.h"

using namespace GOTOEngine;
using GOTOEngine::IRenderFontGlyph;

D2DFont::D2DFont(const wchar_t* fontFamily, IRenderFontStyle style, int size)
    : m_style(style), m_size(size)
{
    DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL;
    DWRITE_FONT_STYLE dwriteStyle = DWRITE_FONT_STYLE_NORMAL;

    if (style == IRenderFontStyle::Bold) weight = DWRITE_FONT_WEIGHT_BOLD;
    if (style == IRenderFontStyle::Italic) dwriteStyle = DWRITE_FONT_STYLE_ITALIC;
    if (style == IRenderFontStyle::BoldItalic) {
        weight = DWRITE_FONT_WEIGHT_BOLD;
        dwriteStyle = DWRITE_FONT_STYLE_ITALIC;
    }

    IDWriteFactory* factory = DWriteHelper::GetFactory();
    factory->CreateTextFormat(
        fontFamily, nullptr,
        weight, dwriteStyle,
        DWRITE_FONT_STRETCH_NORMAL,
        static_cast<FLOAT>(m_size),
        L"ko-kr",
        &m_textFormat
    );
}

D2DFont::~D2DFont()
{
    if (m_textFormat)
        m_textFormat->Release();
}
