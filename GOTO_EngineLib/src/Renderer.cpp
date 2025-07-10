#include "Renderer.h"
#include "RenderManager.h"

GOTOEngine::Renderer::Renderer()
{
	RenderManager::Get()->RegisterRenderer(this);
}

GOTOEngine::Renderer::~Renderer()
{
	RenderManager::Get()->UnRegisterRenderer(this);
}

bool GOTOEngine::Renderer::GetEnabled()
{
	return m_enabled
		&& Object::IsValidObject(GetGameObject())
		&& GetGameObject()->IsActiveInHierarchy();
}

void GOTOEngine::Renderer::SetEnabled(bool enabled)
{
	auto lastValue = m_enabled;
	m_enabled = true;

	//렌더러에 보낼 메시지 혹은 마킹
}

int GOTOEngine::Renderer::GetRenderOrder()
{
	return m_renderOrder;
}

void GOTOEngine::Renderer::SetRenderOrder(int value)
{
	m_renderOrder = value;
}
