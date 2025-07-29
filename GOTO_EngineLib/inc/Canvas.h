#pragma once
#include "Behaviour.h"
#include "Vector2.h"

namespace GOTOEngine
{
	class Graphic;
	class RectTransform;
	class Canvas : public Behaviour
	{
	private:
		friend class RenderManager;
		friend class Graphic;
		friend class GameObject;
		int m_sortOrder = 0; // ������ ����
		std::vector<Graphic*> m_graphics; // �� ĵ������ ���� �׷��ȵ�
		bool m_needGraphicSort;

		void AdditionalInitialize() override;

		void RegisterGraphic(Graphic* graphic);
		void UnregisterGraphic(Graphic* graphic);
		void SortGraphics();
		virtual ~Canvas();
		void Dispose() override
		{
			
		}
	public:
		Canvas();
		void SetSortOrder(int order);
		int GetSortOrder() const { return m_sortOrder; }
		RectTransform* GetRectTransform();
		void OnEnable();
		void OnDisable();
		Vector2 GetCanvasSize() const;
		bool IsNeedGraphicSort() const { return m_needGraphicSort; }
	};
}