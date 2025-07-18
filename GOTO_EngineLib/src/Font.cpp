#include "Font.h"
#include "IRenderFont.h"

void GOTOEngine::Font::LoadFromFilePath(const std::wstring& filePath)
{
    m_fontHandle = RenderManager::Get()->CreateRenderFontFromFilePath(filePath);
    
    //�ȵǴ� ��� �ý��� ��Ʈ�ε� ������
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
/// ���ҽ� �Ŵ��� Load<T>������ ������ �����Ǵ� Font������Ʈ�� �����մϴ�
/// ���� �ν��Ͻ� ������ �������� �ʱ� ������ ���� �����Ͽ����մϴ�.
/// </summary>
/// <param name="fontName"></param>
/// <param name="size"></param>
/// <returns></returns>
//GOTOEngine::Font* GOTOEngine::Font::CreateFontFromOSFont(const std::wstring& fontName, size_t size)
//{
//    auto osFontHandle = RenderManager::Get()->CreateRenderFontFromOS(fontName);
//    if (!osFontHandle) //<---�ش��ϴ� OS��Ʈ�� �������� ����
//        return nullptr;
//    
//    auto osFont = new Font();
//    osFont->m_fontHandle = osFontHandle;
//    osFont->m_filePath = L"RuntimeFont::" + fontName;
//   
//    return osFont;
//}