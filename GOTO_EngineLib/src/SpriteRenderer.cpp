#include "SpriteRenderer.h"
#include "Sprite.h"

void GOTOEngine::SpriteRenderer::Render(Matrix3x3& matrix)
{
}

void GOTOEngine::SpriteRenderer::SetSprite(Sprite* sprite)
{
    if (m_sprite != sprite)
    {
        if (sprite)
            sprite->IncreaseRefCount();
        if (m_sprite)
            m_sprite->DecreaseRefCount();
    }
    m_sprite = sprite;
}
