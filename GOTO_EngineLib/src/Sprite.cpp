#include "Sprite.h"
#include "Texture2D.h"

GOTOEngine::Sprite::Sprite() 
    : m_texture(nullptr)
    , m_rect(Rect{0,0,0,0})
    , m_pivotX(0.5f)
    , m_pivotY(0.5f)
{
}

GOTOEngine::Sprite::Sprite(Texture2D* texture)
    : m_texture(texture)
    , m_rect(Rect{ 0,0,0,0 })
    , m_pivotX(0.5f)
    , m_pivotY(0.5f)
{
}

GOTOEngine::Sprite::~Sprite()
{

}

void GOTOEngine::Sprite::Dispose()
{
    Resource::Dispose();
    if (IsValidObject(m_texture) 
        && !m_texture->IsDestroyed())
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


    if (m_texture && m_rect.width == 0 && m_rect.height == 0)
    {
        m_rect.x = 0;
        m_rect.y = 0;
        m_rect.width = m_texture->GetRect().width;
        m_rect.height = m_texture->GetRect().height;
    }

}

bool GOTOEngine::Sprite::IsValidData()
{
	return m_texture;
}

void GOTOEngine::Sprite::SetTexture(Texture2D* texture)
{
    if (m_texture != texture)
    {
        if (texture)
            texture->IncreaseRefCount();
        if (m_texture)
            m_texture->DecreaseRefCount();
    }
    m_texture = texture;
}



void GOTOEngine::Sprite::SetTexture(const std::wstring& filePath)
{
    SetTexture(Resource::Load<Texture2D>(filePath));
}
