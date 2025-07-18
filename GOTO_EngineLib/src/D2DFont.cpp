#include "D2DFont.h"
#include "DWriteHelper.h"


using namespace GOTOEngine;
using GOTOEngine::IRenderFont;

//D2DFont::D2DFont(const wchar_t* fontFamily, IRenderFontStyle style, int size)
//    : m_style(style), m_size(size)
//{
//    DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL;
//    DWRITE_FONT_STYLE dwriteStyle = DWRITE_FONT_STYLE_NORMAL;
//
//    if (style == IRenderFontStyle::Bold) weight = DWRITE_FONT_WEIGHT_BOLD;
//    if (style == IRenderFontStyle::Italic) dwriteStyle = DWRITE_FONT_STYLE_ITALIC;
//    if (style == IRenderFontStyle::BoldItalic) {
//        weight = DWRITE_FONT_WEIGHT_BOLD;
//        dwriteStyle = DWRITE_FONT_STYLE_ITALIC;
//    }
//
//    IDWriteFactory* factory = DWriteHelper::GetFactory();
//    factory->CreateTextFormat(
//        fontFamily, nullptr,
//        weight, dwriteStyle,
//        DWRITE_FONT_STRETCH_NORMAL,
//        static_cast<FLOAT>(m_size),
//        L"ko-kr",
//        &m_textFormat
//    );
//}

D2DFont::~D2DFont()
{
    for (auto it = m_textFormats.begin(); it != m_textFormats.end(); it++)
    {
        if (it->second)
            it->second = nullptr;
    }
    m_textFormats.clear();
    m_fontCollection = nullptr;
    m_fontFile = nullptr;
}

IDWriteTextFormat* GOTOEngine::D2DFont::GetRaw(size_t size, IRenderFontStyle style) const
{
    std::pair key = {size,style};
    auto it = m_textFormats.find(key);
    if (it != m_textFormats.end())
    {
        return it->second.Get();
    }

    //없으면 텍스트포맷 런타임 생성
    IDWriteTextFormat* newTextFormat = CreateTextFormat(size, style);

    if (newTextFormat)
    {
        m_textFormats[key] = newTextFormat;
    }
    return newTextFormat;
}

IDWriteTextFormat* GOTOEngine::D2DFont::CreateTextFormat(size_t size, IRenderFontStyle style) const
{
    if (!DWriteHelper::GetFactory()) return nullptr; // DirectWrite 팩토리가 유효한지 확인

    IDWriteFactory* dwriteFactory = DWriteHelper::GetFactory();
    IDWriteTextFormat* pTextFormat = nullptr;
    DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_NORMAL;
    DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL;
    DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL;

    // IRenderFontStyle을 DirectWrite 스타일로 매핑
    switch (style)
    {
    case IRenderFontStyle::Normal:
        fontWeight = DWRITE_FONT_WEIGHT_NORMAL;
        fontStyle = DWRITE_FONT_STYLE_NORMAL;
        break;
    case IRenderFontStyle::Bold:
        fontWeight = DWRITE_FONT_WEIGHT_BOLD;
        fontStyle = DWRITE_FONT_STYLE_NORMAL;
        break;
    case IRenderFontStyle::Italic:
        fontWeight = DWRITE_FONT_WEIGHT_NORMAL;
        fontStyle = DWRITE_FONT_STYLE_ITALIC;
        break;
    case IRenderFontStyle::BoldItalic:
        fontWeight = DWRITE_FONT_WEIGHT_BOLD;
        fontStyle = DWRITE_FONT_STYLE_ITALIC;
        break;
    }

    HRESULT hr;

    // m_fontFile이 유효하면 파일 폰트 로직 사용, 그렇지 않으면 OS 폰트 로직 사용
    if (m_fontFile && m_fontCollection) // 파일 폰트의 경우, m_fontFile과 m_fontCollection 모두 유효해야 함
    {
        hr = dwriteFactory->CreateTextFormat(
            m_fontFamilyName.c_str(),
            m_fontCollection.Get(), // 커스텀 폰트 컬렉션 사용
            fontWeight,
            fontStyle,
            fontStretch,
            static_cast<FLOAT>(size),
            L"ko-kr", // 로케일
            &pTextFormat
        );
    }
    else
    {
        // OS 폰트의 경우, 시스템 폰트 컬렉션(nullptr) 사용
        hr = dwriteFactory->CreateTextFormat(
            m_fontFamilyName.c_str(),
            nullptr, // 시스템 폰트 컬렉션 사용 시 nullptr
            fontWeight,
            fontStyle,
            fontStretch,
            static_cast<FLOAT>(size),
            L"ko-kr",
            &pTextFormat
        );
    }

    if (FAILED(hr))
    {
        // DirectWrite 텍스트 포맷 생성 실패 처리
        // 예: 로깅, 오류 코드 반환, 예외 throw
        return nullptr;
    }

    return pTextFormat;
}
