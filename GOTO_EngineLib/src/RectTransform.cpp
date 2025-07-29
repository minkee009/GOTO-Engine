#include "RectTransform.h"
#include "Canvas.h"
#include "Graphic.h"

GOTOEngine::RectTransform::RectTransform()
{
}

GOTOEngine::RectTransform::~RectTransform()
{
}

void GOTOEngine::RectTransform::SetAnchorMin(const Vector2& anchorMin)
{
	m_anchorMin = anchorMin;
}

void GOTOEngine::RectTransform::SetAnchorMax(const Vector2& anchorMax)
{
	m_anchorMax = anchorMax;
}

void GOTOEngine::RectTransform::SetAnchors(const Vector2& anchorMin, const Vector2& anchorMax)
{
	m_anchorMin = anchorMin;
	m_anchorMax = anchorMax;
}

const GOTOEngine::Vector2& GOTOEngine::RectTransform::GetAnchorMin() const
{
	// TODO: 여기에 return 문을 삽입합니다.
	return m_anchorMin;
}

const GOTOEngine::Vector2& GOTOEngine::RectTransform::GetAnchorMax() const
{
	// TODO: 여기에 return 문을 삽입합니다.
	return m_anchorMax;
}

void GOTOEngine::RectTransform::SetAnchoredPosition(const Vector2& position)
{
	m_anchoredPosition = position;
}

const GOTOEngine::Vector2& GOTOEngine::RectTransform::GetAnchoredPosition() const
{
	// TODO: 여기에 return 문을 삽입합니다.
	return m_anchoredPosition;
}

void GOTOEngine::RectTransform::SetSizeDelta(const Vector2& sizeDelta)
{
	m_sizeDelta = sizeDelta;
}

const GOTOEngine::Vector2& GOTOEngine::RectTransform::GetSizeDelta() const
{
	// TODO: 여기에 return 문을 삽입합니다.
	return m_sizeDelta;
}

void GOTOEngine::RectTransform::SetPivot(const Vector2& pivot)
{
	m_pivot = pivot;
}

const GOTOEngine::Vector2& GOTOEngine::RectTransform::GetPivot() const
{
	// TODO: 여기에 return 문을 삽입합니다.
	return m_pivot;
}

void GOTOEngine::RectTransform::SetParent(Transform* parent, bool worldPositionStays)
{
	Transform::SetParent(parent, worldPositionStays);

	auto graphics = GetComponents<Graphic>();
	for (auto graphic : graphics)
	{
		graphic->OnCanvasHierarchyChanged();
	}
}

void GOTOEngine::RectTransform::SetSiblingIndex(size_t index)
{
	Transform::SetSiblingIndex(index);

	auto graphics = GetComponents<Graphic>();
	for (auto graphic : graphics)
	{
		graphic->OnCanvasHierarchyChanged();
	}
}
