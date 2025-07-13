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
        
        auto transform = renderAPI->ConvertToD2DMatrix(GetGameObject()->GetTransform()->GetWorldMatrix());
        renderContext->SetTransform(transform);
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
