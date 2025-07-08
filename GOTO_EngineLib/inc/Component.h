#pragma once
#include "Object.h"
#include "GameObject.h"

namespace GOTOEngine
{
	class Transform;
	class Component : public Object
	{
	private:
		friend class GameObject;
		GameObject* m_gameObject;
	protected:
		Component() : m_gameObject(nullptr) {}
		~Component() 
		{ 
			if (Object::IsValidObject(m_gameObject))
			{
				m_gameObject->UnregisterComponent(this);
				m_gameObject = nullptr;
			}
		}
	public:
		GameObject* GetGameObject() { return m_gameObject; }
		Transform* GetTransform() { return m_gameObject->GetTransform(); }
	};
}
