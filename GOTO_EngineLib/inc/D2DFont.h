#pragma once
#include "IRenderFont.h"
#include "DWriteHelper.h"
#include <unordered_map>
#include <dwrite.h>
#include <string>
#pragma comment(lib, "dwrite.lib")

namespace GOTOEngine
{
    class D2DFont : public IRenderFont
    {
    public:
        D2DFont(std::wstring fontFamilyName) : m_fontFamilyName(fontFamilyName) {}
        ~D2DFont() override;

        IDWriteTextFormat* GetRaw(int size, IRenderFontStyle style = IRenderFontStyle::Normal) const;
    private:
        mutable std::unordered_map<int, IDWriteTextFormat*> m_textFormats;
        IDWriteFontCollection* m_fontCollection;
        IDWriteFontFile* m_fontFile;
        std::wstring m_fontFamilyName;
    };
}

//
//#pragma once
//#include <IRenderBitmap.h>
//#include <string>
//#include <vector>
//#include <Windows.h>
//#include <d2d1.h>
//#include <wrl/client.h>
//
//using namespace Microsoft::WRL;
//
//namespace GOTOEngine
//{
//    class D2DBitmap : public IRenderBitmap
//    {
//    public:
//        D2DBitmap(ComPtr<ID2D1Bitmap> bitmap) : m_bitmap(bitmap) {} //원본 이미지 생성
//        ~D2DBitmap() { m_bitmap = nullptr; }
//        float GetWidth() const override { return m_bitmap.Get() ? m_bitmap->GetSize().width : 0; }
//        float GetHeight() const override { return m_bitmap.Get() ? m_bitmap->GetSize().height : 0; }
//
//        ID2D1Bitmap* GetRaw() const { return m_bitmap.Get(); }
//    private:
//        ComPtr<ID2D1Bitmap> m_bitmap;
//    };
//}
