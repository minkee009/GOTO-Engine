# pragma once
#include "Transform.h"
#include "Vector2.h"
#include "Rect.h"

namespace GOTOEngine
{
    class Canvas;

    class RectTransform : public Transform
    {
    private:
        friend class Canvas;
        friend class Graphic;

        // Anchor 시스템
        Vector2 m_anchorMin;
        Vector2 m_anchorMax;
        Vector2 m_anchoredPosition;
        Vector2 m_sizeDelta;
        Vector2 m_pivot;

        virtual ~RectTransform();
    public:
        RectTransform();
        // Anchor 시스템
        void SetAnchorMin(const Vector2& anchorMin);
        void SetAnchorMax(const Vector2& anchorMax);
        void SetAnchors(const Vector2& anchorMin, const Vector2& anchorMax);
        const Vector2& GetAnchorMin() const;
        const Vector2& GetAnchorMax() const;

        // 위치와 크기
        void SetAnchoredPosition(const Vector2& position);
        const Vector2& GetAnchoredPosition() const;

        void SetSizeDelta(const Vector2& sizeDelta);
        const Vector2& GetSizeDelta() const;

        void SetPivot(const Vector2& pivot);
        const Vector2& GetPivot() const;

        void SetParent(Transform* parent, bool worldPositionStays) override;
        void SetSiblingIndex(size_t index) override;
    };
}