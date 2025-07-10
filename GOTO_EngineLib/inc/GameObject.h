#pragma once
#include <type_traits>
#include <vector>
#include <string>
#include <Object.h>
#include <Scene.h>

namespace GOTOEngine
{
	class Transform;
	class Scene;
	class GameObject final : public Object
	{
	private:
		friend class Engine;
		friend class Scene;
		friend class Component;
		friend class Transform;

		std::vector<Component*> m_components;
		std::string m_tag;
		Transform* m_transform;
		Scene* m_scene;
		bool m_active;
		bool m_activeInHierarchy = true; // Hierarchy���� Ȱ��ȭ ����
		
		void InitInstance();
		void RegisterComponent(Component* comp);
		void UnregisterComponent(Component* comp);
		void UpdateActiveInHierarchy();

		void Dispose() override;

		~GameObject();
		static std::vector<GameObject*> s_allGameObjects;
	public:
		GameObject(std::wstring name);
		GameObject();


		void SetActive(bool active);
		void SetTag(const std::string& tag) { m_tag = tag; }

		bool IsActiveSelf() const { return m_active; }
		bool IsActiveInHierarchy() const { return m_activeInHierarchy; }

		const std::string& GetTag() const { return m_tag; }
		const Scene* GetScene() const { return m_scene; }

		template <typename T>
		T* AddComponent()
		{
			// T�� Component�� �Ļ� Ŭ�����̰ų� Component ��ü���� Ȯ��
			static_assert(std::is_base_of<Component, T>::value, "AddComponent()�� T�� Component�� ��ӹ޾ƾ� �մϴ�.");

			// T�� Transform�� �ƴ��� Ȯ��
			static_assert(!std::is_same<Transform, T>::value, "AddComponent()�� Transform�� �߰��� �� �����ϴ�.");

			T* newComponent = new T();
			newComponent->m_gameObject = this;
			RegisterComponent(newComponent);

			return newComponent;
		}

		template <typename T>
		T* GetComponent() const
		{
			// T�� Component�� �Ļ� Ŭ�����̰ų� Component ��ü���� Ȯ��
			static_assert(std::is_base_of<Component, T>::value, "GetComponent()�� T�� Component�� ��ӹ޾ƾ� �մϴ�.");

			for (auto comp : m_components)
			{
				if (T* typedComp = dynamic_cast<T*>(comp))
					return typedComp;
			}

			return nullptr;
		}

		Transform* const GetTransform() { return m_transform; }

		unsigned int layer = 1;

		static GameObject* Find(const std::wstring& name);
		static GameObject* FindWithTag(const std::string& name);
		static std::vector<GameObject*> FindGameObjectsWithTag(const std::string& name);
	};
}

