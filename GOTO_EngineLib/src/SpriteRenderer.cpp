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
        auto transform = Matrix3x3::Translate(spriteRect.width * -m_sprite->GetPivotX(), spriteRect.height * -m_sprite->GetPivotY());

        //����Ƽ ��ǥ�� �̹��� �ø�
        transform = Matrix3x3::Scale(1.0f, -1.0f) * transform;

        ////TRS ����
        transform = GetGameObject()->GetTransform()->GetWorldMatrix() * transform;

        ////����Ƽ ��ǥ�� ��Ʈ���� ����
        transform = matrix * transform;

        renderAPI->DrawBitmap(bitmap, transform, spriteRect);
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
