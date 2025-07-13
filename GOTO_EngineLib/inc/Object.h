#pragma once
#include "Vector2.h"
#include "string"
#include "SceneManager.h"
#include <unordered_set>
#include "IDisopose.h"

namespace GOTOEngine
{
	class Object : public IDispose
	{
	private:
		friend class Engine;
		friend class ObjectDestructionManager;
		long long m_instanceID;
		bool m_isDestroyed = false; // 오브젝트가 파괴되었는지 여부
		void MarkDestory() { m_isDestroyed = true; }

		static std::atomic<long long> s_nextInstanceId;
		static std::vector<Object*> s_registry; // 탐색용 레지스트리 (생성/파괴 주기 관리 X)
		static std::unordered_set<Object*> s_validObjects; // 유효한 오브젝트들 (생성/파괴 주기 관리용)
	protected:
		virtual void Dispose() override { }

		virtual ~Object()
		{
			auto it = std::find(s_registry.begin(), s_registry.end(), this);
			if (it != s_registry.end())
			{
				*it = std::move(s_registry.back()); // 마지막 원소를 덮어씀
				s_registry.pop_back();
			}

			s_validObjects.erase(this); // 유효한 오브젝트 목록에서 제거
		}
		
		Object() : m_instanceID(s_nextInstanceId++)
		{
			name = L"<Unnamed> [ Instance ID : " + std::to_wstring(m_instanceID) + L" ]";
			s_registry.push_back(this); // 레지스트리에 추가
			s_validObjects.insert(this); // 유효한 오브젝트로 등록
		}
		Object(std::wstring name) : name(name), m_instanceID(s_nextInstanceId++)
		{
			s_registry.push_back(this); // 레지스트리에 추가
			s_validObjects.insert(this); // 유효한 오브젝트로 등록
		}
	public:
		std::wstring name;

		long long GetInstanceID() { return m_instanceID; }
		bool IsDestroyed() const { return m_isDestroyed; }

		//static Object* Instantiate(const Object& src);
		static void Destroy(Object* obj, float delay = 0.0f);
		static void DestroyImmediate(Object* obj);

		static void DontDestroyOnLoad(Object* obj);
		template <typename T>
		static T* FindObjectByType() 
		{
			for (Object* obj : s_registry)
			{
				if (T* castedObj = dynamic_cast<T*>(obj))
				{
					return castedObj;
				}
			}
			return nullptr; // 찾지 못한 경우 nullptr 반환
		}
		template <typename T>
		static std::vector<T*> FindObjectsByType() 
		{
			//T타입을 상속받는 오브젝트를 모두 찾아 배열로 반환
			std::vector<T*> foundObjects;
			for (Object* obj : s_registry)
			{
				if (T* castedObj = dynamic_cast<T*>(obj))
				{
					foundObjects.push_back(castedObj);
				}
			}
			return foundObjects;
		}

		static bool IsValidObject(Object* obj)
		{
			// 유효한 오브젝트인지 확인
			return s_validObjects.find(obj) != s_validObjects.end();
		}

		//복사 생성자 , 대입 연산자 삭제
		Object(const Object&) = delete;
		Object& operator=(const Object&) = delete;
	};
}
