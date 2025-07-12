#include "ObjectDestructionManager.h"

void GOTOEngine::ObjectDestructionManager::ScheduleDestroy(Object* obj, float delay)
{
	//��ȿ�� ������Ʈ���� Ȯ��
	if (!Object::IsValidObject(obj)
		|| obj->Destroyed())
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

void GOTOEngine::ObjectDestructionManager::ImmediateDestroy(Object* obj)
{
	//��ȿ�� ������Ʈ���� Ȯ��
	if (!Object::IsValidObject(obj)
		|| obj->Destroyed())
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
	
	//���ݺ��� ��ǻ� �ı��Ǿ����� ����
	obj->MarkDestory();
	obj->Dispose();
	m_pendingDeleteObjects.push_back(obj); // ��� �ı��� ������Ʈ ��Ͽ� �߰�
}

void GOTOEngine::ObjectDestructionManager::Update()
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

			//���ݺ��� ��ǻ� �ı��Ǿ����� ����
			obj->MarkDestory();
			obj->Dispose();
			m_pendingDeleteObjects.push_back(obj); // ��� �ı��� ������Ʈ ��Ͽ� �߰�
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

void GOTOEngine::ObjectDestructionManager::Clear()
{
	for (auto& obj : m_pendingDeleteObjects)
	{
		delete obj; // ���� �ı�
	}
	m_pendingDeleteObjects.clear(); // �ı��� ������Ʈ ��� �ʱ�ȭ
}

void GOTOEngine::ObjectDestructionManager::ShutDown()
{
	// ��� ���� �ı��� ������Ʈ�� �ı�
	for (auto& pair : m_destroySchedule)
	{
		//������ �ı����ุ �ϰ� �ı�
		ObjectDestroyInfo* destroyInfo = pair.second;
		if (destroyInfo)
		{
			if (!Object::IsValidObject(destroyInfo->obj) 
				|| destroyInfo->obj->Destroyed())
				continue; // ��ȿ���� ���� ������Ʈ�� ����
			
			destroyInfo->obj->Dispose();
			destroyInfo->obj->m_isDestroyed = true;
			m_pendingDeleteObjects.push_back(destroyInfo->obj);
			delete destroyInfo; // ���� ������ ����
		}
	}
	m_destroySchedule.clear();
	Clear();
}
