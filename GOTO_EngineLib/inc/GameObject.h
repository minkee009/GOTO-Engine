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
		bool m_activeInHierarchy = true; // Hierarchy에서 활성화 여부
		
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
			// T가 Component의 파생 클래스이거나 Component 자체인지 확인
			static_assert(std::is_base_of<Component, T>::value, "AddComponent()의 T는 Component를 상속받아야 합니다.");

			// T가 Transform이 아닌지 확인
			static_assert(!std::is_same<Transform, T>::value, "AddComponent()로 Transform을 추가할 수 없습니다.");

			T* newComponent = new T();
			newComponent->m_gameObject = this;
			RegisterComponent(newComponent);

			return newComponent;
		}

		template <typename T>
		T* GetComponent() const
		{
			// T가 Component의 파생 클래스이거나 Component 자체인지 확인
			static_assert(std::is_base_of<Component, T>::value, "GetComponent()의 T는 Component를 상속받아야 합니다.");

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

