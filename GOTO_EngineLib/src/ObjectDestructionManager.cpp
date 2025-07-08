#include "ObjectDestructionManager.h"

void GOTOEngine::ObjectDestructionManager::ScheduleDestroy(Object* obj, float delay)
{
	//유효한 오브젝트인지 확인
	if (!Object::IsValidObject(obj))
		return;

	//Transform의 경우엔 Destroy불가능
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

	// 예약 파괴 정보 생성
	ObjectDestroyInfo* destroyInfo = new ObjectDestroyInfo(obj, destroyTime);
	m_destroySchedule[obj] = destroyInfo;
}

void GOTOEngine::ObjectDestructionManager::ImmediateDestroy(Object* obj)
{
	//유효한 오브젝트인지 확인
	if (!Object::IsValidObject(obj))
		return;

	//Transform의 경우엔 Destroy불가능
	if (dynamic_cast<Transform*>(obj))
	{
		return;
	}

	//예약된 오브젝트의 경우 파괴예약을 취소하고 즉시 파괴
	auto it = m_destroySchedule.find(obj);
	if (it != m_destroySchedule.end())
	{
		// 예약정보 훼손
		delete it->second;
		m_destroySchedule[obj] = nullptr;
	}

	if (auto behaviour = dynamic_cast<Behaviour*>(obj))
	{
		//Behaviour인 경우 BehaviourManager에서 제거
		if (behaviour->IsActiveAndEnabled())
			behaviour->CallBehaviourMessage("OnDisable");
		behaviour->CallBehaviourMessage("OnDestroy");
	}

	delete obj; // 실제 파괴
}

void GOTOEngine::ObjectDestructionManager::Update()
{
	// 현재 시간 기준으로 파괴예약된 오브젝트들 중 파괴시간이 지난 오브젝트들을 직접파괴
	float currentTime = TIME_GET_TOTALTIME();

	// 파괴 시간이 지나지 않은 오브젝트들은 그대로 두기
	for (auto it = m_destroySchedule.begin(); it != m_destroySchedule.end(); )
	{
		ObjectDestroyInfo* destroyInfo = it->second;
		if (destroyInfo && destroyInfo->delayedTime <= currentTime)
		{
			// 예약 파괴 시간이 지난 오브젝트를 파괴
			Object* obj = destroyInfo->obj;

			if (auto behaviour = dynamic_cast<Behaviour*>(obj))
			{
				//Behaviour인 경우 BehaviourManager에서 제거
				if (behaviour->IsActiveAndEnabled())
					behaviour->CallBehaviourMessage("OnDisable");
				behaviour->CallBehaviourMessage("OnDestroy");
			}

			delete obj; // 실제 파괴
			delete destroyInfo; // 예약 정보도 삭제
			it = m_destroySchedule.erase(it); // 맵에서 제거
		}
		else
		{
			++it; // 다음 요소로 이동
		}
	}

	// 즉시파괴로 비정상적으로 사라진 파괴예약정보에 대한 제거
	for (auto it = m_destroySchedule.begin(); it != m_destroySchedule.end(); )
	{
		ObjectDestroyInfo* destroyInfo = it->second;
		if (!destroyInfo)
		{
			it = m_destroySchedule.erase(it); // 맵에서 제거
		}
		else
		{
			++it; // 다음 요소로 이동
		}
	}
}

void GOTOEngine::ObjectDestructionManager::ShutDown()
{
	// 모든 예약 파괴된 오브젝트들 파괴
	for (auto& pair : m_destroySchedule)
	{
		//안전한 파괴예약만 일괄 파괴
		ObjectDestroyInfo* destroyInfo = pair.second;
		if (destroyInfo)
		{
			if (auto behaviour = dynamic_cast<Behaviour*>(destroyInfo->obj))
			{
				//Behaviour인 경우 BehaviourManager에서 제거
				if (behaviour->GetEnabled())
					behaviour->CallBehaviourMessage("OnDisable");
				behaviour->CallBehaviourMessage("OnDestroy");
			}
			delete destroyInfo->obj; // 실제 파괴
			delete destroyInfo; // 예약 정보도 삭제
		}
	}
	m_destroySchedule.clear();
}
