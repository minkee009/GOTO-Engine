#include "GameObject.h"
#include "Component.h"
#include "ObjectDestructionManager.h"
#include "Transform.h"

std::vector<GOTOEngine::GameObject*> GOTOEngine::GameObject::s_allGameObjects;

void GOTOEngine::GameObject::InitInstance()
{
	//transform�� ���� ���� �� m_components�� ���
	m_transform = new Transform();
	m_transform->m_gameObject = this;
	m_transform->SetParent(nullptr); // �θ� �����Ƿ� nullptr�� ����

	RegisterComponent(m_transform); // Transform�� ������Ʈ�� ���

	UpdateActiveInHierarchy();

	s_allGameObjects.push_back(this); // ��� ���� ������Ʈ ��Ͽ� �߰�
}

void GOTOEngine::GameObject::RegisterComponent(Component* comp)
{
	m_components.push_back(comp);
}

void GOTOEngine::GameObject::UnregisterComponent(Component* comp)
{
	auto it = std::find(m_components.begin(), m_components.end(), comp);
	if (it != m_components.end()) {
		*it = std::move(m_components.back()); // ������ ���Ҹ� ���
		m_components.pop_back();
	}
}

void GOTOEngine::GameObject::UpdateActiveInHierarchy()
{
	if (Transform* parent = m_transform->GetParent())
	{
		m_activeInHierarchy = parent->GetGameObject()->IsActiveInHierarchy() && m_active;
	}
	else
	{
		m_activeInHierarchy = m_active; // �θ� ������ �ڱ� �ڽŸ� Ȱ��ȭ ���θ� ����
	}

	// �ڽĵ��� Ȱ��ȭ ���� ����
	for (auto& child : m_transform->m_childs)
	{
		child->GetGameObject()->UpdateActiveInHierarchy();
	}
}

void GOTOEngine::GameObject::Dispose()
{
	Object::Dispose();

	////====== Ʈ���� �� �ı� ======////
	// 
	// 
	//Ʈ�������� ���ϵ�鵵 �ϰ� �ı��ؾ���...
	for (auto& child : m_transform->m_childs)
	{
		DestroyImmediate(child->GetGameObject()); //
	}
	//
	//
	////===========================////



	////====== ������Ʈ �ı� =======////
	//
	//
	for (auto& comp : m_components)
	{
		if (!Object::IsValidObject(comp))
			continue; // �̹� �ı��� ������Ʈ�� ����
		DestroyImmediate(comp);
	}
	//
	//
	////===========================////
}

GOTOEngine::GameObject::GameObject(std::wstring name)
	: Object(name)
	, m_tag("")
	, m_active(true)
{
	m_scene = SceneManager::Get() ? SCENE_GET_CURRENTSCENE() : nullptr;
	m_scene->RegisterGameObject(this);

	InitInstance();
}

GOTOEngine::GameObject::GameObject() 
	: m_tag("")
	, m_active(true)
{
	m_scene = SceneManager::Get() ? SCENE_GET_CURRENTSCENE() : nullptr;
	m_scene->RegisterGameObject(this);

	InitInstance();
}

GOTOEngine::GameObject::~GameObject()
{
	////====== Ʈ���� �� �ı� ======////
	// 
	// 
	//Ʈ�������� ���ϵ�鵵 �ϰ� �ı��ؾ���...
	for (auto& child : m_transform->m_childs)
	{
		if (!Object::IsValidObject(child))
			continue; // �̹� �ı��� ���ϵ�� ����
		//iterator ���� ���� -> RemoveChild()����;
		child->m_parent = nullptr;
	}
	m_transform->m_childs.clear(); // �ڽ� Ʈ������ ��� �ʱ�ȭ

	if (Object::IsValidObject(m_transform->m_parent))
		m_transform->m_parent->RemoveChild(m_transform);

	delete m_transform; // Transform ������Ʈ �ı�
	m_transform = nullptr;
	//
	//
	////===========================////



	////====== ������Ʈ �ı� =======////
	//
	//
	for (auto& comp : m_components)
	{
		if (!Object::IsValidObject(comp))
			continue; // �̹� �ı��� ������Ʈ�� ����

		//iterator ���� ���� -> UnregisterComponent()ȣ�� ����
		comp->m_gameObject = nullptr;
	}
	m_components.clear();
	//
	//
	////===========================////

	////====== �� ��� ���� ========////
	if (m_scene)
	{
		m_scene->UnregisterGameObject(this);
		m_scene = nullptr;
	}
	////===========================////

	// ��� ���� ������Ʈ ��Ͽ��� ����
	auto it = std::find(s_allGameObjects.begin(), s_allGameObjects.end(), this);
	if (it != s_allGameObjects.end())
	{
		*it = std::move(s_allGameObjects.back()); // ������ ���Ҹ� ���
		s_allGameObjects.pop_back();
	}
}

GOTOEngine::GameObject::GameObject(Scene* scene) 
	: m_scene(scene)
	, m_tag("")
	, m_active(true)
{
	m_scene->RegisterGameObject(this);
	InitInstance();
}

GOTOEngine::GameObject::GameObject(Scene* scene, std::wstring name)
	: m_scene(scene)
	, m_tag("")
	, m_active(true)
{
	m_scene->RegisterGameObject(this);
	InitInstance();
}

void GOTOEngine::GameObject::SetActive(bool active)
{
	m_active = active;

	UpdateActiveInHierarchy();
}

GOTOEngine::GameObject* GOTOEngine::GameObject::Find(const std::wstring& name)
{
	//s_allGameObjects���� �̸����� �˻�
	for (auto& go : s_allGameObjects)
	{
		if (!go->IsActiveInHierarchy()
			|| go->IsDestroyed())
			continue;

		if (go->name == name)
		{
			return go;
		}
	}

	return nullptr;
}

GOTOEngine::GameObject* GOTOEngine::GameObject::FindWithTag(const std::string& name)
{
	//s_allGameObjects���� �±׷� �˻�
	for (auto& go : s_allGameObjects)
	{
		if (!go->IsActiveInHierarchy()
			|| go->IsDestroyed())
			continue;

		if (go->GetTag() == name)
		{
			return go;
		}
	}

	return nullptr;
}

std::vector<GOTOEngine::GameObject*> GOTOEngine::GameObject::FindGameObjectsWithTag(const std::string& name)
{
	auto GOs = std::vector<GameObject*>();

	for (auto& go : s_allGameObjects)
	{
		if (!go->IsActiveInHierarchy()
			|| go->IsDestroyed())
			continue;

		if (go->GetTag() == name)
		{
			GOs.emplace_back(go);
		}
	}

	return GOs;
}

//bool GOTOEngine::GameObject::IsActiveInHierarchy() const
//{
//	if (!m_active) return false;
//	
//	if (Transform* parent = m_transform->GetParent()) 
//		return parent->GetGameObject()->IsActiveInHierarchy();
//	
//	return true;
//}
