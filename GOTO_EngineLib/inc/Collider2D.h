#pragma once
#include "Component.h"
#include "PhysicsManager.h"
#include "Vector2.h"
#include "box2d-lite/Body.h"

namespace GOTOEngine
{
	class Collider2D : public Component
	{
	private:
		friend class PhysicsManager;
		friend class PhysicsManager::Body2DWrapper;
		PhysicsManager::Body2DWrapper* m_wrapperBody;
		Vector2 m_size;

		PhysicsManager::Body2DWrapper* GetWrapperBody() { return m_wrapperBody; };
		

		~Collider2D();
	public:
		Collider2D();


		Vector2 GetSize() 
		{ 
			 if(GetWrapperBody() && GetWrapperBody()->m_pBody )
				 return  { GetWrapperBody()->m_pBody->width.x,m_wrapperBody->m_pBody->width.y};

			 return m_size;
		}
		void SetSize(const Vector2& size) 
		{ 
			if (GetWrapperBody() && GetWrapperBody()->m_pBody)
				GetWrapperBody()->m_pBody->width = { size.x, size.y };
			else
				m_size = size;
		}
	};
}