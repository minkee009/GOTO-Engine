#include "Font.h"
#include "IRenderFont.h"

void GOTOEngine::Font::LoadFromFilePath(const std::wstring& filePath)
{
    m_fontHandle = RenderManager::Get()->CreateRenderFontFromFilePath(filePath);
}

GOTOEngine::Font::~Font()
{
    if (m_fontHandle)
    {
        delete m_fontHandle;
        m_fontHandle = nullptr;
    }
}

GOTOEngine::Font* GOTOEngine::Font::CreateFontFromOSFont(const std::wstring& fontName, size_t size)
{

    return nullptr;
}