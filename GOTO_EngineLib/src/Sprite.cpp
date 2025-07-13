#include "Sprite.h"
#include "Texture2D.h"

GOTOEngine::Sprite::Sprite() 
    : m_texture(nullptr)
    , m_width(0)
    , m_height(0)
    , m_pivotX(0.5f)
    , m_pivotY(0.5f)
{
}

GOTOEngine::Sprite::~Sprite()
{

}

void GOTOEngine::Sprite::Dispose()
{
    Object::Dispose();
    if (m_texture)
    {
        m_texture->DecreaseRefCount();
        m_texture = nullptr;
    }
}
void GOTOEngine::Sprite::LoadFromFilePath(const std::wstring& filePath)
{
    auto lastTexture = m_texture;
    m_texture = Load<Texture2D>(filePath);

    if (m_texture != lastTexture)
    {
        if (m_texture)
            m_texture->IncreaseRefCount();
        if (lastTexture)
            lastTexture->DecreaseRefCount();
    }


    if (m_texture && m_width == 0 && m_height == 0)
    {
        m_width = m_texture->GetWidth();
        m_height = m_texture->GetHeight();
    }

}

bool GOTOEngine::Sprite::IsValidRawData()
{
	return m_texture;
}
