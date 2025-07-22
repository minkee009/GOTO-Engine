#include "SpriteRenderer.h"
#include "RenderManager.h"
#include "Sprite.h"
#include "Texture2D.h"
#include "D2DRenderAPI.h"
#include "D2DBitmap.h"
#include "Transform.h"
#include "GameObject.h"

void GOTOEngine::SpriteRenderer::Render(Matrix3x3& matrix)
{
    auto renderAPI = GetRenderAPIFromManager();

    if (m_sprite && m_sprite->m_texture)
    {
        auto bitmap = m_sprite->m_texture->GetBitmap();

        auto spriteRect = m_sprite->GetRect();

        //�ǹ� �̵�
        auto transform = Matrix3x3::Translate(spriteRect.width * -m_sprite->GetPivotX(),spriteRect.height * m_sprite->GetPivotY() - spriteRect.height);

        //����Ƽ ��ǥ�� �̹��� �ø�
        transform = Matrix3x3::Scale(m_flipX ? -1.0f : 1.0f, m_flipY ? 1.0f : -1.0f) * transform;

        ////TRS ����
        transform = GetGameObject()->GetTransform()->GetWorldMatrix() * transform;

        ////����Ƽ ��ǥ�� ��Ʈ���� ����
        transform = matrix * transform;

        TextureFiltering filter = TextureFiltering::Nearest;
        switch (m_sprite->m_texture->GetRenderMode())
        {
        case TextureRenderMode::Point:
            filter = TextureFiltering::Nearest;
            break;  
        case TextureRenderMode::Bilinear:
            filter = TextureFiltering::Linear;
            break;
        }

        renderAPI->DrawBitmap(bitmap, transform, { 0,0,spriteRect.width,spriteRect.height }, spriteRect, filter, false);
    }
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

void GOTOEngine::SpriteRenderer::SetSprite(Sprite* sprite, Rect srcRect)
{
    SetSprite(sprite);
    if (m_sprite)
    {
        m_sprite->SetRect(srcRect);
    }
}
