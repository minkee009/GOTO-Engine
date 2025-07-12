#include "Texture2D.h"
#include "RenderManager.h"

void GOTOEngine::Texture2D::LoadFromFilePath(const std::wstring& filePath)
{
	m_bitmapHandle = RenderManager::Get()->CreateBitmap(filePath);
}

GOTOEngine::Texture2D::Texture2D() : m_bitmapHandle(nullptr)
{
}

GOTOEngine::Texture2D::~Texture2D()
{
	if (m_bitmapHandle)
		delete m_bitmapHandle;
}

bool GOTOEngine::Texture2D::IsValidRawData()
{
	return m_bitmapHandle;
}
