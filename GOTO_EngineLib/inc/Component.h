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
			if (IsValidObject(m_gameObject))
			{
				m_gameObject->UnregisterComponent(this);
				m_gameObject = nullptr;
			}
		}
	public:
		GameObject* GetGameObject() { return m_gameObject; }
		Transform* GetTransform() { return m_gameObject->GetTransform(); }

		template <typename T>
		T* GetComponent()
		{
			if (IsValidObject(GetGameObject())
				&& !GetGameObject()->IsDestroyed())
			{
				return GetGameObject()->GetComponent<T>();
			}
			return nullptr;
		}

		template <typename T>
		T* AddComponent()
		{
			if (IsValidObject(GetGameObject())
				&& !GetGameObject()->IsDestroyed())
			{
				return GetGameObject()->AddComponent<T>();
			}
			return nullptr;
		}
	};
}
