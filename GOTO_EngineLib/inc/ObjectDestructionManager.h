#pragma once
#include "Singleton.h"
#include "Transform.h"
#include "Object.h"
#include "TimeManager.h"
#include <vector>
#include <unordered_map>

namespace GOTOEngine
{
	class ObjectDestructionManager : public Singleton<ObjectDestructionManager>
	{
	private:
		friend class Object;
		friend class Engine;
		struct ObjectDestroyInfo
		{
			Object* obj;
			float delayedTime; // ���� ��ü�ð� + �����̽ð�

			ObjectDestroyInfo(Object* object, float delayTime)
				: obj(object), delayedTime(delayTime) {
			}
		};
		std::unordered_map<Object*, ObjectDestroyInfo*> m_destroySchedule; // Instance ID�� Ű�� �ϴ� ���� �ı� ����
		std::vector<Object*> m_pendingDeleteObjects; // ���� ������ų ������Ʈ��

		//������Ʈ �ı��� �����ϴ� �Լ�
		void ScheduleDestroy(Object* obj, float delay);

		//�������(Ȥ�� ���������) ��� �ı��ϴ� �Լ�
		void ImmediateDestroy(Object* obj);

		void Update();
		void Clear();
		void StartUp() { }
		void ShutDown();

	public:

		ObjectDestructionManager() = default;
 	};
}