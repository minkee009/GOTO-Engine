#include "Texture2D.h"
#include "RenderManager.h"

void GOTOEngine::Texture2D::LoadFromFilePath(const std::wstring& filePath)
{
	m_bitmapHandle = RenderManager::Get()->CreateRenderBitmap(filePath);
	if (m_bitmapHandle)
	{
		m_rect.width = m_bitmapHandle->GetWidth();
		m_rect.height = m_bitmapHandle->GetHeight();
	}
}

GOTOEngine::Texture2D::Texture2D() : m_bitmapHandle(nullptr), m_rect(Rect{0,0,0,0})
{
}

GOTOEngine::Texture2D::~Texture2D()
{
	if (m_bitmapHandle)
		delete m_bitmapHandle;
}
