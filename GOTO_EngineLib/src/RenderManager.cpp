#include "RenderManager.h"
#include "IRenderAPI.h"
#include "IWindow.h"
#include "Camera.h"
#include "Delegate.h"
#include "D2DRenderAPI.h"
#include "Renderer.h"
#include "algorithm"
#include "Matrix3x3.h"
#ifdef _DEBUG
#include <iostream>
#endif

#include "BehaviourManager.h"
#undef CreateFont
using namespace GOTOEngine;

void RenderManager::DrawString(float x, float y, float width, float height, const wchar_t* text, const IRenderFont* font, bool rightAlign, Color color)
{
	m_pRenderAPI->DrawString(x, y, width, height, text, font, rightAlign, color);
}

void RenderManager::StartUp(IWindow* window)
{
#ifdef _USE_DIRECT2D
	m_pRenderAPI = new D2DRenderAPI();
#endif

	if (!m_pRenderAPI->Initialize(window))
	{
		delete m_pRenderAPI;
		m_pRenderAPI = nullptr;
	}
	else
	{
		static_cast<IWindow*>(window)->SetOnChangedWindowSize([this](int width, int height) {
			m_pRenderAPI->ChangeBufferSize(width, height);
#ifdef _DEBUG
			std::cout << width << "," << height << std::endl;
#endif 
			});
	}
}

RenderAPIMemoryStatus RenderManager::CollectMemoryUsage()
{
	return m_pRenderAPI->CollectMemoryUsage();
}

void RenderManager::ShutDown()
{
	m_cameras.clear();

	if (m_pRenderAPI)
	{
		delete m_pRenderAPI;
		m_pRenderAPI = nullptr;
	}
}

void GOTOEngine::RenderManager::RegisterCamera(Camera* cam)
{
	m_cameras.push_back(cam);
	SetCamSortDirty();
}

void GOTOEngine::RenderManager::UnRegisterCamera(Camera* cam)
{
	m_cameras.erase(
		std::remove_if(m_cameras.begin(), m_cameras.end(),
			[cam](const auto& item) { return item == cam; }),
		m_cameras.end());

	SetCamSortDirty();
}

void GOTOEngine::RenderManager::RegisterRenderer(Renderer* renderer)
{
	m_renderers.push_back(renderer);
	SetRendererSortDirty();
}

void GOTOEngine::RenderManager::UnRegisterRenderer(Renderer* renderer)
{
	m_renderers.erase(
		std::remove_if(m_renderers.begin(), m_renderers.end(),
			[renderer](const auto& item) { return item == renderer; }),
		m_renderers.end());
	SetRendererSortDirty();
}

void GOTOEngine::RenderManager::SortCamera()
{
	std::sort(m_cameras.begin(), m_cameras.end(),
		[](Camera* a, Camera* b) {
			return a->GetDepth() < b->GetDepth();
		});
}

void GOTOEngine::RenderManager::SortRenderer()
{
	std::sort(m_renderers.begin(), m_renderers.end(),
		[](Renderer* a, Renderer* b) {
			return a->m_renderOrder < b->m_renderOrder;
		});
}

void GOTOEngine::RenderManager::StartRender()
{
	m_pRenderAPI->Clear();
}

void GOTOEngine::RenderManager::EndRender()
{
	m_pRenderAPI->SwapBuffer();
}

IRenderBitmap* GOTOEngine::RenderManager::CreateBitmap(std::wstring filePath)
{
	return m_pRenderAPI->CreateBitmap(filePath);
}

GOTOEngine::IRenderFont* GOTOEngine::RenderManager::CreateFont(std::wstring filePath)
{
	return nullptr;
}

void RenderManager::SetVSyncInterval(int interval)
{
	if (m_pRenderAPI)
	{
		m_pRenderAPI->SetVSyncInterval(interval);
	}
}

void GOTOEngine::RenderManager::Render()
{
	//활성카메라, 비활성카메라 설정
	//CheckActiveCamera();
	//CheckActiveRenderer();
	//둘 중 하나라도 없는 경우 렌더 종료
	//if(m_activeCamera.size() == 0 || m_activeRenderer.size() == 0) { Clear(); SwapBuffer(); return; }

	if(m_needCamDepthSort)
		SortCamera();

	if (m_needRenderOrderSort)
		SortRenderer();

	//렌더링
	//멀티 카메라를 구현하려면 렌더타겟(백 버퍼)이 카메라마다 존재해야함
	//활성화된 카메라를 돌면서 Clear -> 렌더링해주고 최종 렌더타겟을 하나에 모아서 Composite(합치기)
	//그리고 그 렌더타겟을 스왑체인이나 메인버퍼로 올려주고 플립핑

	for (const auto& camera : m_cameras)
	{
		if (!camera->GetEnabled())
			continue;

		//카메라 행렬 구하기
		auto camRect = camera->GetRect();
		Matrix3x3 cameraMat = camera->GetMatrix();

		//Todo : 그리기 전에 카메라 영역 박스색칠 (렌더타겟이 없기 때문에 클리어 대신 씀)
		m_pRenderAPI->SetViewport(camRect);
		m_pRenderAPI->DrawRect(
			m_pRenderAPI->GetWindow().GetWidth() * camRect.x,
			m_pRenderAPI->GetWindow().GetHeight() * camRect.y,
			m_pRenderAPI->GetWindow().GetWidth() * camRect.width,
			m_pRenderAPI->GetWindow().GetHeight() * camRect.height,
			true,
			camera->GetBackGroundColor());
	
		
		for (const auto& renderer : m_renderers)
		{
			if (!renderer->GetEnabled()
				|| (renderer->GetRenderLayer() & camera->GetRenderLayer()) == 0)
				continue;

			//뷰포트 제한
			renderer->Render(cameraMat);
		}
		m_pRenderAPI->ResetViewport();
	}
}

const GOTOEngine::IWindow* GOTOEngine::RenderManager::GetWindow() const
{
	return &m_pRenderAPI->GetWindow();
}

