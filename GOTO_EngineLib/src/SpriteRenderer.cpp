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
    auto renderAPI = dynamic_cast<D2DRenderAPI*>(RenderManager::Get()->GetRenderAPI());
    auto renderContext = renderAPI->GetContext();

    if (m_sprite && m_sprite->m_texture)
    {
        auto bitmap = dynamic_cast<D2DBitmap*>(m_sprite->m_texture->GetBitmap())->GetRaw();
        
        auto transform = Matrix3x3::Translate(m_sprite->GetWidth() * -m_sprite->GetPivotX(), m_sprite->GetHeight() * -m_sprite->GetPivotY());
        //����Ƽ ��ǥ�� �̹��� �ø�
        transform = Matrix3x3::Scale(1.0f, -1.0f) * transform;

        ////TRS ����
        transform = GetGameObject()->GetTransform()->GetWorldMatrix() * transform;

        ////����Ƽ ��ǥ�� ��Ʈ���� ����
        transform = matrix * transform;

        auto d2dtransform = renderAPI->ConvertToD2DMatrix(transform);

        //transform = transform * D2D1::Matrix3x2F::Translation(renderAPI->GetClipRect().left, renderAPI->GetClipRect().top) ;
        renderContext->SetTransform(d2dtransform);
        renderContext->DrawBitmap(bitmap);
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
