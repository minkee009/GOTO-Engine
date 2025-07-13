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

using namespace GOTOEngine;

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
		dynamic_cast<IWindow*>(window)->SetOnChangedWindowSize([this](int width, int height) {
			m_pRenderAPI->ChangeBufferSize(width, height);
#ifdef _DEBUG
			std::cout << width << "," << height << std::endl;
#endif 
			});
	}
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

IRenderBitmap* GOTOEngine::RenderManager::CreateBitmap(std::wstring filePath)
{
	return m_pRenderAPI->CreateBitmap(filePath);
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
	//Ȱ��ī�޶�, ��Ȱ��ī�޶� ����
	//CheckActiveCamera();
	//CheckActiveRenderer();
	//�� �� �ϳ��� ���� ��� ���� ����
	//if(m_activeCamera.size() == 0 || m_activeRenderer.size() == 0) { Clear(); SwapBuffer(); return; }

	if(m_needCamDepthSort)
		SortCamera();

	if (m_needRenderOrderSort)
		SortRenderer();

	m_pRenderAPI->Clear();

	//������
	//��Ƽ ī�޶� �����Ϸ��� ����Ÿ��(�� ����)�� ī�޶󸶴� �����ؾ���
	//Ȱ��ȭ�� ī�޶� ���鼭 Clear -> ���������ְ� ���� ����Ÿ���� �ϳ��� ��Ƽ� Composite(��ġ��)
	//�׸��� �� ����Ÿ���� ����ü���̳� ���ι��۷� �÷��ְ� �ø���

	for (const auto& camera : m_cameras)
	{
		if (!camera->GetEnabled())
			continue;

		//ī�޶� ��� ���ϱ�
		auto camRect = camera->GetRect();
		Matrix3x3 unityCoordMat = 
		    Matrix3x3::Translate(m_pRenderAPI->GetWindow().GetWidth() * (camRect.x + (camRect.width * 0.5f)) , 
				m_pRenderAPI->GetWindow().GetHeight() * (camRect.y + (camRect.height * 0.5f)))
			* Matrix3x3::Scale(1.0f, -1.0f) 
			* camera->GetMatrix();

		//Todo : �׸��� ���� ī�޶� ���� �ڽ���ĥ (����Ÿ���� ���� ������ Ŭ���� ��� ��)
		// -> ī�޶� ���� (solid color) �����ϱ�
		// -> �߰��� ī�޶� ������ �̹� �ٸ� ī�޶󿵿��� ���� �Ϻ��� �������� ���� ī�޶� ���������� �Ѿ��
		// ---->> �ռ� �׸� ī�޶� ������ �ִ�ũ�⸦ �������� ���� �� ���� �����ϰ� ���� ī�޶� ���������� ���ϱ�
		//---�ڵ�
		
		for (const auto& renderer : m_renderers)
		{
			if (!renderer->GetEnabled()
				|| (renderer->GetRenderLayer() & camera->GetRenderLayer()) == 0)
				continue;

			//����Ʈ ����
			m_pRenderAPI->SetViewport(camRect);
			renderer->Render(unityCoordMat);
			m_pRenderAPI->ResetViewport();
		}
	}
	m_pRenderAPI->SwapBuffer();
}

const GOTOEngine::IWindow* GOTOEngine::RenderManager::GetWindow() const
{
	return &m_pRenderAPI->GetWindow();
}

