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

    //������ �ؽ�Ʈ���� ��Ÿ�� ����
    IDWriteTextFormat* newTextFormat = CreateTextFormat(size, style);

    if (newTextFormat)
    {
        m_textFormats[key] = newTextFormat;
    }
    return newTextFormat;
}

IDWriteTextFormat* GOTOEngine::D2DFont::CreateTextFormat(size_t size, IRenderFontStyle style) const
{
    if (!DWriteHelper::GetFactory()) return nullptr; // DirectWrite ���丮�� ��ȿ���� Ȯ��

    IDWriteFactory* dwriteFactory = DWriteHelper::GetFactory();
    IDWriteTextFormat* pTextFormat = nullptr;
    DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_NORMAL;
    DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL;
    DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL;

    // IRenderFontStyle�� DirectWrite ��Ÿ�Ϸ� ����
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

    // m_fontFile�� ��ȿ�ϸ� ���� ��Ʈ ���� ���, �׷��� ������ OS ��Ʈ ���� ���
    if (m_fontFile && m_fontCollection) // ���� ��Ʈ�� ���, m_fontFile�� m_fontCollection ��� ��ȿ�ؾ� ��
    {
        hr = dwriteFactory->CreateTextFormat(
            m_fontFamilyName.c_str(),
            m_fontCollection.Get(), // Ŀ���� ��Ʈ �÷��� ���
            fontWeight,
            fontStyle,
            fontStretch,
            static_cast<FLOAT>(size),
            L"ko-kr", // ������
            &pTextFormat
        );
    }
    else
    {
        // OS ��Ʈ�� ���, �ý��� ��Ʈ �÷���(nullptr) ���
        hr = dwriteFactory->CreateTextFormat(
            m_fontFamilyName.c_str(),
            nullptr, // �ý��� ��Ʈ �÷��� ��� �� nullptr
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
        // DirectWrite �ؽ�Ʈ ���� ���� ���� ó��
        // ��: �α�, ���� �ڵ� ��ȯ, ���� throw
        return nullptr;
    }

    return pTextFormat;
}
