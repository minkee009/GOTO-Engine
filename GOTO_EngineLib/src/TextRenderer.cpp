#include "TextRenderer.h"
#include "Transform.h"
#include "IRenderAPI.h"

void GOTOEngine::TextRenderer::Render(Matrix3x3& matrix)
{
    auto renderAPI = GetRenderAPIFromManager();

    if (m_font)
    {
        auto fontHandle = m_font->GetFont();

        //피벗 이동
        auto transform = Matrix3x3::Translate(rect.width * -m_pivot.x, rect.height * -m_pivot.y);

        ////TRS 세팅
        transform = GetGameObject()->GetTransform()->GetWorldMatrix() * transform;

        ////유니티 좌표계 매트릭스 적용
        transform = matrix * transform;

        renderAPI->DrawString(text.c_str(), rect, fontHandle, size, FontStyleHelper::ToRenderFontStyle(fontStyle), color, transform, static_cast<int>(horizontalAlign), static_cast<int>(verticalAlign));
    }
}

void GOTOEngine::TextRenderer::SetFont(Font* font)
{
    if (m_font != font)
    {
        if (font)
            font->IncreaseRefCount();
        if (m_font)
            m_font->DecreaseRefCount();
    }
    m_font = font;
}