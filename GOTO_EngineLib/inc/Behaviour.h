#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Delegate.h"

namespace GOTOEngine
{
	//루프 제어에 영향받는 컴포넌트입니다.
	class Behaviour : public Component
	{
	protected:
		bool m_enabled;
	public:
		bool GetEnabled() const
		{
			return m_enabled;
		}
		void SetEnabled(bool value)
		{
			if (value != m_enabled)
			{
				//바뀔 때 무언갈 실행하는 코드 작성하기
				m_enabled = value;
			}
		}
		bool IsActiveAndEnabled()
		{
			return m_enabled && GetGameObject()->IsActiveInHierarchy(); // Object::IsValidObject(GetGameObject()) &&;
		}
	};
}
