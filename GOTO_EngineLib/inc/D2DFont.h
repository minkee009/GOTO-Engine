#pragma once
#include "IRenderFont.h"
#include "DWriteHelper.h"
#include <unordered_map>
#include <dwrite.h>
#include <string>
#pragma comment(lib, "dwrite.lib")

#include <functional>
#include <wrl/client.h>

using namespace Microsoft::WRL;

//페어 키 정의
namespace std
{
    template <>
    struct hash<std::pair<size_t, GOTOEngine::IRenderFontStyle>>
    {
        size_t operator()(const std::pair<size_t, GOTOEngine::IRenderFontStyle>& pair) const noexcept
        {
            return std::hash<size_t>()(pair.first) ^ (std::hash<GOTOEngine::IRenderFontStyle>()(pair.second) << 1);
        }
    };
}

namespace GOTOEngine
{
    class D2DFont : public IRenderFont
    {
    public:
        D2DFont(std::wstring fontFamilyName) 
            : m_fontFamilyName(fontFamilyName)
            , m_fontCollection(nullptr)
            , m_fontFile(nullptr) 
        {

        }

        ~D2DFont() override;

        IDWriteTextFormat* GetRaw(size_t size, IRenderFontStyle style = IRenderFontStyle::Normal) const;
    private:
        friend class D2DRenderAPI;

        mutable std::unordered_map<std::pair<size_t, IRenderFontStyle> , ComPtr<IDWriteTextFormat>> m_textFormats;
        ComPtr<IDWriteFontCollection> m_fontCollection;
        ComPtr<IDWriteFontFile> m_fontFile;
        std::wstring m_fontFamilyName;

        IDWriteTextFormat* CreateTextFormat(size_t size, IRenderFontStyle style) const;
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
