#pragma once
#include "Behaviour.h"

namespace GOTOEngine 
{
	class ScriptBehaviour : public Behaviour
	{
	public:
		ScriptBehaviour() = default;
		virtual ~ScriptBehaviour() = default;
		// ��ũ��Ʈ �ʱ�ȭ
		virtual void Start() {}
		// �� ������ ������Ʈ
		virtual void Update(float deltaTime) {}
		// ���� ������Ʈ (���� ���� ��)
		virtual void FixedUpdate(float fixedDeltaTime) {}
		// ������ �� ȣ��
		virtual void PreRender() {}
		// ������ �� ȣ��
		virtual void PostRender() {}
		// ��ũ��Ʈ ����
		virtual void OnDestroy() {}
	};
}