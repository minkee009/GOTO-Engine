#pragma once
#include "Singleton.h"
#include "Transform.h"
#include "Object.h"
#include "TimeManager.h"
#include "Behaviour.h"
#include <vector>
#include <unordered_map>

namespace GOTOEngine
{
	class ObjectDestructionManager : public Singleton<ObjectDestructionManager>
	{
	private:
		friend class Object;
		struct ObjectDestroyInfo
		{
			Object* obj;
			float delayedTime; // ���� ��ü�ð� + �����̽ð�

			ObjectDestroyInfo(Object* object, float delayTime)
				: obj(object), delayedTime(delayTime) {
			}
		};
		std::unordered_map<Object*, ObjectDestroyInfo*> m_destroySchedule; // Instance ID�� Ű�� �ϴ� ���� �ı� ����

		//������Ʈ �ı��� �����ϴ� �Լ�
		void ScheduleDestroy(Object* obj, float delay);

		//������� ��� �ı�(�������� �������)�ϴ� �Լ�
		void ImmediateDestroy(Object* obj);

	public:
		ObjectDestructionManager() = default;

		void Update();
		void StartUp() { }
		void ShutDown();
 	};
}