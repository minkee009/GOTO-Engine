#include "RenderManager.h"
#include "IRenderImage.h"
#include "IRenderAPI.h"
#include "IWindow.h"
#include "Delegate.h"
#include "D2DRenderer.h"
#ifdef _DEBUG
#include <iostream>
#endif

using namespace GOTOEngine;

void RenderManager::StartUp(IWindow* window)
{
#ifdef _USE_DIRECT2D
	m_pRenderer = new D2DRenderer();
#endif

	if (!m_pRenderer->Initialize(window))
	{
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}
	else
	{
		dynamic_cast<IWindow*>(window)->SetOnChangedWindowSize([this](int width, int height) {
			m_pRenderer->ChangeBufferSize(width, height);
#ifdef _DEBUG
			std::cout << width << "," << height << std::endl;
#endif 
			});
	}
}

void RenderManager::Clear()
{
	if (m_pRenderer)
	{
		m_pRenderer->Clear();
	}
}

void RenderManager::DrawImage(int x, int y, float scale, bool flipX, const IRenderImage* image)
{
	if (m_pRenderer)
	{
		m_pRenderer->DrawImage(x, y, scale, flipX, image);
	}
}

void RenderManager::DrawImage(int x, int y, float scale, const IRenderImage* image)
{
	DrawImage(x, y, scale, false, image);
}

void RenderManager::DrawImage(int x, int y, const IRenderImage* image)
{
	DrawImage(x, y, 1.0f, false, image);
}

void RenderManager::DrawImage(Vector2 pos, float scale, bool flipX, const IRenderImage* image)
{
	DrawImage(static_cast<int>(pos.x), static_cast<int>(pos.y), scale, flipX, image);
}

void RenderManager::DrawImage(Vector2 pos, float scale, const IRenderImage* image)
{
	DrawImage(static_cast<int>(pos.x), static_cast<int>(pos.y), scale, false, image);
}

void RenderManager::DrawImage(Vector2 pos, const IRenderImage* image)
{
	DrawImage(static_cast<int>(pos.x), static_cast<int>(pos.y), 1.0f, false, image);
}

void RenderManager::DrawRect(int x, int y, int width, int height, bool fill, Color color)
{
	if (m_pRenderer)
	{
		m_pRenderer->DrawRect(x, y, width, height, fill, color);
	}
}

void RenderManager::DrawRect(int x, int y, int width, int height, Color color)
{
	DrawRect(x, y, width, height, false, color);
}

void GOTOEngine::RenderManager::DrawString(int x, int y, int width, int height, const wchar_t* string, const IRenderFont* font, bool rightAlign, Color color)
{
	if (m_pRenderer)
	{
		m_pRenderer->DrawString(x, y, width, height, string, font, rightAlign, color);
	}
}

void GOTOEngine::RenderManager::DrawString(int x, int y, int width, int height, const wchar_t* string, const IRenderFont* font, Color color)
{
	DrawString(x, y, width, height, string, font, false, color);
}


void RenderManager::DrawString(int x, int y, const wchar_t* string, const IRenderFont* font, bool rightAlign, Color color)
{
	DrawString(x, y, m_pRenderer->GetWindow().GetWidth(), m_pRenderer->GetWindow().GetHeight(), string, font, rightAlign, color);
}

void RenderManager::DrawString(int x, int y, const wchar_t* string, const IRenderFont* font, Color color)
{
	DrawString(x, y, m_pRenderer->GetWindow().GetWidth(), m_pRenderer->GetWindow().GetHeight(), string, font, false, color);
}

void RenderManager::SwapBuffer()
{
	if (m_pRenderer)
	{
		m_pRenderer->SwapBuffer();
	}
}

void RenderManager::ShutDown()
{
	if (m_pRenderer)
	{
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}
}

void RenderManager::SetVSyncInterval(int interval)
{
	if (m_pRenderer)
	{
		m_pRenderer->SetVSyncInterval(interval);
	}
}

