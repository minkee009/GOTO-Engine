#include "GameObject.h"
#include "Component.h"
#include "ObjectDestructionManager.h"
#include "Transform.h"

void GOTOEngine::GameObject::InitInstance()
{
	m_scene = SceneManager::Get() ? SCENE_GET_CURRENTSCENE() : nullptr;
	m_scene->RegisterGameObject(this);

	//transform�� ���� ���� �� m_components�� ���
	m_transform = new Transform();
	m_transform->m_gameObject = this;
	m_transform->SetParent(nullptr); // �θ� �����Ƿ� nullptr�� ����

	RegisterComponent(m_transform); // Transform�� ������Ʈ�� ���
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

GOTOEngine::GameObject::GameObject(std::wstring name)
	: Object(name)
	, m_tag(L"")
	, m_active(true)
{
	InitInstance();
}

GOTOEngine::GameObject::GameObject() 
	: m_tag(L"")
	, m_active(true)
{
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
		//iterator ���� ���� -> RemoveChild()����;
		child->m_parent = nullptr;
		Object::DestroyImmediate(child->GetGameObject()); // ��� �ı�
	}
	m_transform->m_childs.clear(); // �ڽ� Ʈ������ ��� �ʱ�ȭ

	if (m_transform->m_parent)
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

		Object::DestroyImmediate(comp);
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
}

bool GOTOEngine::GameObject::IsActiveInHierarchy() const
{
	if (!m_active) return false;
	
	if (Transform* parent = m_transform->GetParent()) 
		return parent->GetGameObject()->IsActiveInHierarchy();
	
	return true;
}
