#pragma once
#include "UIBehaviour.h"
#include "Color.h"

namespace GOTOEngine
{
    class IRenderAPI;
	class Graphic : public UIBehaviour
	{
	private:
        friend class RectTransform;
    protected:
		Canvas* m_canvas;
		Color m_color;
		bool m_raycastTarget;
		int m_sortOrder = 0; // ������ ����

        void OnEnable();
        void OnDisable();
        void OnCanvasHierarchyChanged() override;
        IRenderAPI* GetRenderAPIFromManager();
    protected:
        void AdditionalInitialize() override;
	public:
        Graphic();
        virtual ~Graphic();

        void Dispose() override;

        Canvas* FindCanvasInParents();

        // ����
        void SetColor(const Color& color);
        const Color& GetColor() const;

        // Raycast Target
        void SetRaycastTarget(bool raycastTarget);
        bool GetRaycastTarget() const;

        // Canvas�� RectTransform ����
        Canvas* GetCanvas() const;
		void SetCanvas(Canvas* canvas) { m_canvas = canvas; }
        RectTransform* GetRectTransform();
		void SetSortOrder(int order) { m_sortOrder = order; }
		int GetSortOrder() const { return m_sortOrder; }

        // ������ (Canvas���� ȣ��)
        virtual void Render() = 0;
	};
}