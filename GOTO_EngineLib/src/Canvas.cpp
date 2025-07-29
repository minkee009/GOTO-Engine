#include "Canvas.h"
#include "Graphic.h"
#include "RenderManager.h"
#include "RectTransform.h"
#include "IWindow.h"
#include <algorithm>


void GOTOEngine::Canvas::RegisterGraphic(Graphic* graphic)
{
	if (graphic && std::find(m_graphics.begin(), m_graphics.end(), graphic) == m_graphics.end())
	{
		m_graphics.push_back(graphic);
	}
	m_needGraphicSort = true; // 그래픽이 추가되었으므로 정렬 필요
}

void GOTOEngine::Canvas::UnregisterGraphic(Graphic* graphic)
{
	if (graphic)
	{
		auto it = std::remove(m_graphics.begin(), m_graphics.end(), graphic);
		if (it != m_graphics.end())
		{
			m_graphics.erase(it, m_graphics.end());
		}
	}
	if (m_graphics.empty())
	{
		m_needGraphicSort = false; // 그래픽이 모두 제거되었으므로 정렬 필요 없음
	}
	else
	{
		m_needGraphicSort = true; // 그래픽이 제거되었으므로 정렬 필요
	}
}


void GOTOEngine::Canvas::SortGraphics()
{
	std::sort(m_graphics.begin(), m_graphics.end(),
		[](Graphic* a, Graphic* b) {
			return a->GetSortOrder() < b->GetSortOrder();
		});

	m_needGraphicSort = false;
}

GOTOEngine::Canvas::~Canvas()
{
	RenderManager::Get()->UnRegisterCanvas(this);
	for (auto& graphic : m_graphics)
	{
		if (IsValidObject(graphic)
			&& !graphic->IsDestroyed())
		{
			graphic->SetCanvas(nullptr);
		}
	}
	m_graphics.clear();
}

GOTOEngine::Canvas::Canvas() : m_needGraphicSort(false)
{
	RenderManager::Get()->RegisterCanvas(this);
	REGISTER_BEHAVIOUR_MESSAGE(OnEnable);
	REGISTER_BEHAVIOUR_MESSAGE(OnDisable);
}

void GOTOEngine::Canvas::AdditionalInitialize()
{
	RectTransform* rectTransform = GetRectTransform();
	if (!rectTransform)
	{
		rectTransform->SetAnchorMax({ 0,0 });
		rectTransform->SetAnchorMin({ 0,0 });
		rectTransform->SetPivot({ 0.5f, 0.5f });

		auto size = GetCanvasSize();

		rectTransform->SetSizeDelta(size);
		rectTransform->SetPosition(size * 0.5f);
	}
}

void GOTOEngine::Canvas::SetSortOrder(int order)
{
	if (m_sortOrder != order)
	{
		m_sortOrder = order;
		RenderManager::Get()->SetCanvasSortDirty();
	}
}

GOTOEngine::RectTransform* GOTOEngine::Canvas::GetRectTransform()
{
	return dynamic_cast<RectTransform*>(GetGameObject()->GetTransform());
}

void GOTOEngine::Canvas::OnEnable()
{
}

void GOTOEngine::Canvas::OnDisable()
{
}

GOTOEngine::Vector2 GOTOEngine::Canvas::GetCanvasSize() const
{
	return Vector2{
			static_cast<float>(RenderManager::Get()->GetWindow()->GetWidth()),
			static_cast<float>(RenderManager::Get()->GetWindow()->GetHeight())
	};
}
