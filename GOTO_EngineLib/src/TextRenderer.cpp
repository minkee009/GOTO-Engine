#include "TextRenderer.h"

void GOTOEngine::TextRenderer::Render(Matrix3x3& matrix)
{
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