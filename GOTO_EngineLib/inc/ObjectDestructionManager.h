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
		void ScheduleDestroy(Object* obj, float delay)
		{
			//��ȿ�� ������Ʈ���� Ȯ��
			if (!Object::IsValidObject(obj))
				return;

			//Transform�� ��쿣 Destroy�Ұ���
			if (dynamic_cast<Transform*>(obj))
			{
				return;
			}

			float currentTime = TIME_GET_TOTALTIME();
			float destroyTime = currentTime + delay;

			auto it = m_destroySchedule.find(obj);

			if (m_destroySchedule.end() != it)
			{
				if (it->second->delayedTime > destroyTime)
					it->second->delayedTime = destroyTime;
				return;
			}

			// ���� �ı� ���� ����
			ObjectDestroyInfo* destroyInfo = new ObjectDestroyInfo(obj, destroyTime);
			m_destroySchedule[obj] = destroyInfo;
		}

		//������� ��� �ı�(�������� �������)�ϴ� �Լ�
		void ImmediateDestroy(Object* obj)
		{
			//��ȿ�� ������Ʈ���� Ȯ��
			if (!Object::IsValidObject(obj))
				return;

			//Transform�� ��쿣 Destroy�Ұ���
			if (dynamic_cast<Transform*>(obj))
			{
				return;
			}

			//����� ������Ʈ�� ��� �ı������� ����ϰ� ��� �ı�
			auto it = m_destroySchedule.find(obj);
			if (it != m_destroySchedule.end())
			{
				// �������� �Ѽ�
				delete it->second; 
				m_destroySchedule[obj] = nullptr;
			}

			if (auto behaviour = dynamic_cast<Behaviour*>(obj))
			{
				//Behaviour�� ��� BehaviourManager���� ����
				if(behaviour->IsActiveAndEnabled())
					behaviour->CallBehaviourMessage("OnDisable");
				behaviour->CallBehaviourMessage("OnDestroy");
			}

			delete obj; // ���� �ı�
		}



	public:
		ObjectDestructionManager() = default;

		void Update()
		{
			// ���� �ð� �������� �ı������ ������Ʈ�� �� �ı��ð��� ���� ������Ʈ���� �����ı�
			float currentTime = TIME_GET_TOTALTIME();

			// �ı� �ð��� ������ ���� ������Ʈ���� �״�� �α�
			for (auto it = m_destroySchedule.begin(); it != m_destroySchedule.end(); )
			{
				ObjectDestroyInfo* destroyInfo = it->second;
				if (destroyInfo && destroyInfo->delayedTime <= currentTime)
				{
					// ���� �ı� �ð��� ���� ������Ʈ�� �ı�
					Object* obj = destroyInfo->obj;

					if (auto behaviour = dynamic_cast<Behaviour*>(obj))
					{
						//Behaviour�� ��� BehaviourManager���� ����
						if (behaviour->IsActiveAndEnabled())
							behaviour->CallBehaviourMessage("OnDisable");
						behaviour->CallBehaviourMessage("OnDestroy");
					}

					delete obj; // ���� �ı�
					delete destroyInfo; // ���� ������ ����
					it = m_destroySchedule.erase(it); // �ʿ��� ����
				}
				else
				{
					++it; // ���� ��ҷ� �̵�
				}
			}

			// ����ı��� ������������ ����� �ı����������� ���� ����
			for (auto it = m_destroySchedule.begin(); it != m_destroySchedule.end(); )
			{
				ObjectDestroyInfo* destroyInfo = it->second;
				if (!destroyInfo)
				{
					it = m_destroySchedule.erase(it); // �ʿ��� ����
				}
				else
				{
					++it; // ���� ��ҷ� �̵�
				}
			}
		}

		void StartUp()
		{

		}

		void ShutDown()
		{
			// ��� ���� �ı��� ������Ʈ�� �ı�
			for (auto& pair : m_destroySchedule)
			{
				//������ �ı����ุ �ϰ� �ı�
				ObjectDestroyInfo* destroyInfo = pair.second;
				if (destroyInfo)
				{
					if (auto behaviour = dynamic_cast<Behaviour*>(destroyInfo->obj))
					{
						//Behaviour�� ��� BehaviourManager���� ����
						if (behaviour->IsActiveAndEnabled())
							behaviour->CallBehaviourMessage("OnDisable");
						behaviour->CallBehaviourMessage("OnDestroy");
					}
					delete destroyInfo->obj; // ���� �ı�
					delete destroyInfo; // ���� ������ ����
				}
			}
			m_destroySchedule.clear();
		}
 	};
}