#include "TextRenderer.h"
#include "Transform.h"
#include "IRenderAPI.h"

void GOTOEngine::TextRenderer::Render(Matrix3x3& matrix)
{
    auto renderAPI = GetRenderAPIFromManager();

    IRenderFont* fontHandle = nullptr;
    if(m_font)
        fontHandle = m_font->GetFont();

    //�ǹ� �̵�
    auto transform = Matrix3x3::Translate(rect.width * -pivot.x, rect.height * -pivot.y);

    //����Ƽ ��ǥ�� �ø�
    transform = Matrix3x3::Scale(1.0f, -1.0f) * transform;

    ////TRS ����
    transform = GetGameObject()->GetTransform()->GetWorldMatrix() * transform;

    ////����Ƽ ��ǥ�� ��Ʈ���� ����
    transform = matrix * transform;

    renderAPI->DrawString(text.c_str(), rect, fontHandle, size, FontStyleHelper::ToRenderFontStyle(fontStyle), color, transform, static_cast<int>(horizontalAlign), static_cast<int>(verticalAlign), false);
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