#include "Renderer.h"
#include "RenderManager.h"

GOTOEngine::Renderer::Renderer() : m_enabled(true), m_renderOrder(0), m_renderLayer(static_cast<size_t>(-1))
{
	RenderManager::Get()->RegisterRenderer(this);
}

GOTOEngine::Renderer::~Renderer()
{
	RenderManager::Get()->UnRegisterRenderer(this);
}

GOTOEngine::IRenderAPI* GOTOEngine::Renderer::GetRenderAPIFromManager()
{
	return RenderManager::Get()->GetRenderAPI();
}

bool GOTOEngine::Renderer::GetEnabled()
{
	return m_enabled
		&& IsValidObject(GetGameObject())
		&& GetGameObject()->IsActiveInHierarchy();
}

void GOTOEngine::Renderer::SetEnabled(bool enabled)
{
	auto lastValue = m_enabled;
	m_enabled = true;

	//�������� ���� �޽��� Ȥ�� ��ŷ
}

int GOTOEngine::Renderer::GetRenderOrder()
{
	return m_renderOrder;
}

void GOTOEngine::Renderer::SetRenderOrder(int value)
{
	m_renderOrder = value;
}
