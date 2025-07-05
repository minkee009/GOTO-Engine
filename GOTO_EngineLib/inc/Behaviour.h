#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Delegate.h"

namespace GOTOEngine
{
	//���� ��� ����޴� ������Ʈ�Դϴ�.
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
				//�ٲ� �� ���� �����ϴ� �ڵ� �ۼ��ϱ�
				m_enabled = value;
			}
		}
		bool IsActiveAndEnabled()
		{
			return m_enabled && GetGameObject()->IsActiveInHierarchy(); // Object::IsValidObject(GetGameObject()) &&;
		}
	};
}
