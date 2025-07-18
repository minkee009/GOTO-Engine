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

        //피벗 이동
        auto transform = Matrix3x3::Translate(spriteRect.width * -m_sprite->GetPivotX(), spriteRect.height * -m_sprite->GetPivotY());

        //유니티 좌표계 이미지 플립
        transform = Matrix3x3::Scale(1.0f, -1.0f) * transform;

        ////TRS 세팅
        transform = GetGameObject()->GetTransform()->GetWorldMatrix() * transform;

        ////유니티 좌표계 매트릭스 적용
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
