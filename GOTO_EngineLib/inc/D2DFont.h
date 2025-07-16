#pragma once
#include "IRenderFontGlyph.h"
#include <dwrite.h>
#pragma comment(lib, "dwrite.lib")

namespace GOTOEngine
{
    class D2DFont : public IRenderFontGlyph
    {
    public:
        D2DFont(const wchar_t* fontFamily, IRenderFontStyle style, int size);
        ~D2DFont() override;

        void* GetNativeHandle() const override { return m_textFormat; }
        IRenderFontStyle GetStyle() const override { return m_style; }
        IDWriteTextFormat* GetTextFormat() const { m_textFormat; }
        int GetSize() const override { return m_size; }

    private:
        IRenderFontStyle m_style;
        int m_size;
        IDWriteTextFormat* m_textFormat = nullptr;
    };
}
