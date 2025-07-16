#include "Font.h"

void GOTOEngine::Font::LoadFromFilePath(const std::wstring& filePath)
{

}

bool GOTOEngine::Font::IsValidRawData()
{
    if (m_fontHandles.find(m_size) != m_fontHandles.end())
        return true;

    return false;
}

GOTOEngine::Font* GOTOEngine::Font::CreateFontFromOSFont(const std::wstring& fontName)
{


    return nullptr;
}
