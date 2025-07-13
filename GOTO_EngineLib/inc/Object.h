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
		bool m_isDestroyed = false; // ������Ʈ�� �ı��Ǿ����� ����
		void MarkDestory() { m_isDestroyed = true; }

		static std::atomic<long long> s_nextInstanceId;
		static std::vector<Object*> s_registry; // Ž���� ������Ʈ�� (����/�ı� �ֱ� ���� X)
		static std::unordered_set<Object*> s_validObjects; // ��ȿ�� ������Ʈ�� (����/�ı� �ֱ� ������)
	protected:
		virtual void Dispose() override { }

		virtual ~Object()
		{
			auto it = std::find(s_registry.begin(), s_registry.end(), this);
			if (it != s_registry.end())
			{
				*it = std::move(s_registry.back()); // ������ ���Ҹ� ���
				s_registry.pop_back();
			}

			s_validObjects.erase(this); // ��ȿ�� ������Ʈ ��Ͽ��� ����
		}
		
		Object() : m_instanceID(s_nextInstanceId++)
		{
			name = L"<Unnamed> [ Instance ID : " + std::to_wstring(m_instanceID) + L" ]";
			s_registry.push_back(this); // ������Ʈ���� �߰�
			s_validObjects.insert(this); // ��ȿ�� ������Ʈ�� ���
		}
		Object(std::wstring name) : name(name), m_instanceID(s_nextInstanceId++)
		{
			s_registry.push_back(this); // ������Ʈ���� �߰�
			s_validObjects.insert(this); // ��ȿ�� ������Ʈ�� ���
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
			return nullptr; // ã�� ���� ��� nullptr ��ȯ
		}
		template <typename T>
		static std::vector<T*> FindObjectsByType() 
		{
			//TŸ���� ��ӹ޴� ������Ʈ�� ��� ã�� �迭�� ��ȯ
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
			// ��ȿ�� ������Ʈ���� Ȯ��
			return s_validObjects.find(obj) != s_validObjects.end();
		}

		//���� ������ , ���� ������ ����
		Object(const Object&) = delete;
		Object& operator=(const Object&) = delete;
	};
}
