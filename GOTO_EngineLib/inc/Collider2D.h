#pragma once
#include "Component.h"
#include "PhysicsManager.h"
#include "Vector2.h"
#include "box2d-lite/Body.h"

namespace GOTOEngine
{
	class PhysicsManager::Body2DWrapper;
	class Collider2D : public Component
	{
	private:
		PhysicsManager::Body2DWrapper* m_wrapperBody;

		PhysicsManager::Body2DWrapper* GetWrapperBody() { return m_wrapperBody; };
		~Collider2D();
	public:
		Collider2D();


		Vector2 GetSize() 
		{ 
			 if(GetWrapperBody() && GetWrapperBody()->m_pBody )
				 return  { GetWrapperBody()->m_pBody->width.x,m_wrapperBody->m_pBody->width.y};

			 return { 0,0 };
		}
		void SetSize(const Vector2& size) 
		{ 
			if (GetWrapperBody() && GetWrapperBody()->m_pBody)
				GetWrapperBody()->m_pBody->width = { size.x, size.y };
		}
	};
}