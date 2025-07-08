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
			float delayedTime; // 엔진 전체시간 + 딜레이시간

			ObjectDestroyInfo(Object* object, float delayTime)
				: obj(object), delayedTime(delayTime) {
			}
		};
		std::unordered_map<Object*, ObjectDestroyInfo*> m_destroySchedule; // Instance ID를 키로 하는 예약 파괴 정보
		std::vector<Object*> m_pendingDeleteObjects; // 실제 삭제시킬 오브젝트들

		//오브젝트 파괴를 예약하는 함수
		void ScheduleDestroy(Object* obj, float delay);

		//예약없이(혹은 예약지우고) 즉시 파괴하는 함수
		void ImmediateDestroy(Object* obj);

		void Update();
		void Clear();
		void StartUp() { }
		void ShutDown();

	public:

		ObjectDestructionManager() = default;
 	};
}