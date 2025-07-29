#include "Graphic.h"
#include "Canvas.h"
#include "RectTransform.h"
#include "RenderManager.h"
#include "IRenderAPI.h"

GOTOEngine::Canvas* GOTOEngine::Graphic::FindCanvasInParents()
{
    // 1. 본인이 Canvas를 갖고 있다면 반환
    if (auto selfCanvas = GetComponent<Canvas>())
        return selfCanvas;

    // 2. 부모 Transform을 따라 올라가며 Canvas 탐색
    auto transform = GetTransform();
    while (transform && transform->GetParent())
    {
        transform = transform->GetParent();

        if (auto canvas = transform->GetComponent<Canvas>())
            return canvas;
    }

    return nullptr;
}

GOTOEngine::Graphic::Graphic() : m_canvas(nullptr), m_color(255, 255, 255, 255), m_raycastTarget(true)
{
    REGISTER_BEHAVIOUR_MESSAGE(OnEnable);
    REGISTER_BEHAVIOUR_MESSAGE(OnDisable);
}

GOTOEngine::Graphic::~Graphic()
{
}

void GOTOEngine::Graphic::Dispose()
{
}

void GOTOEngine::Graphic::OnEnable()
{
    
}

void GOTOEngine::Graphic::OnDisable()
{
}

void GOTOEngine::Graphic::OnCanvasHierarchyChanged()
{
	auto foundCanvas = FindCanvasInParents();

    if (m_canvas != foundCanvas)
    {
		if (m_canvas)
		{
			m_canvas->UnregisterGraphic(this);
		}
		m_canvas = foundCanvas;
		if (m_canvas)
		{
			m_canvas->RegisterGraphic(this);
		}
    }
}

GOTOEngine::IRenderAPI* GOTOEngine::Graphic::GetRenderAPIFromManager()
{
	return RenderManager::Get()->GetRenderAPI();
}

void GOTOEngine::Graphic::AdditionalInitialize()
{
    OnCanvasHierarchyChanged();
}

void GOTOEngine::Graphic::SetColor(const Color& color)
{
}

const GOTOEngine::Color & GOTOEngine::Graphic::GetColor() const
{
    // TODO: 여기에 return 문을 삽입합니다.
    return m_color;
}

void GOTOEngine::Graphic::SetRaycastTarget(bool raycastTarget)
{
}

bool GOTOEngine::Graphic::GetRaycastTarget() const
{
    return false;
}

GOTOEngine::Canvas* GOTOEngine::Graphic::GetCanvas() const
{
    return m_canvas;
}

GOTOEngine::RectTransform* GOTOEngine::Graphic::GetRectTransform()
{
    return dynamic_cast<RectTransform*>(GetGameObject()->GetTransform());
}

