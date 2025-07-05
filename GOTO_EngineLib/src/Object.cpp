#include "Object.h"
#include "TimeManager.h"
#include "ObjectDestructionManager.h"

std::atomic<long long> GOTOEngine::Object::s_nextInstanceId = 0;
std::vector<GOTOEngine::Object*> GOTOEngine::Object::s_registry;
std::unordered_set<GOTOEngine::Object*> GOTOEngine::Object::s_validObjects;

//메모리 직렬화/역직렬화로 복사해야 함
//연결한 직렬화 라이브러리가 없기 때문에 보류
//GameEngine::Object* GameEngine::Object::Instantiate(const Object& src)
//{
//	return new Object(src.name);
//}

void GOTOEngine::Object::Destroy(Object* obj, float delay)
{
	ObjectDestructionManager::Get()->ScheduleDestroy(obj, delay);
}

void GOTOEngine::Object::DestroyImmediate(Object* obj)
{
	ObjectDestructionManager::Get()->ImmediateDestroy(obj);
}
