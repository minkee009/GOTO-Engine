#include "Font.h"
#include "IRenderFont.h"

void GOTOEngine::Font::LoadFromFilePath(const std::wstring& filePath)
{
    m_fontHandle = RenderManager::Get()->CreateRenderFontFromFilePath(filePath);
    
    //안되는 경우 시스템 폰트로도 만들어보기
    if (!m_fontHandle)
        m_fontHandle = RenderManager::Get()->CreateRenderFontFromOS(filePath);
}

GOTOEngine::Font::~Font()
{
    if (m_fontHandle)
    {
        delete m_fontHandle;
        m_fontHandle = nullptr;
    }
}

/// <summary>
/// 리소스 매니저 Load<T>로직과 별개로 관리되는 Font오브젝트를 생성합니다
/// 단일 인스턴스 제공을 지원하지 않기 때문에 사용시 유의하여야합니다.
/// </summary>
/// <param name="fontName"></param>
/// <param name="size"></param>
/// <returns></returns>
//GOTOEngine::Font* GOTOEngine::Font::CreateFontFromOSFont(const std::wstring& fontName, size_t size)
//{
//    auto osFontHandle = RenderManager::Get()->CreateRenderFontFromOS(fontName);
//    if (!osFontHandle) //<---해당하는 OS폰트가 존재하지 않음
//        return nullptr;
//    
//    auto osFont = new Font();
//    osFont->m_fontHandle = osFontHandle;
//    osFont->m_filePath = L"RuntimeFont::" + fontName;
//   
//    return osFont;
//}