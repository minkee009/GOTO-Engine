#include "GameObject.h"
#include "Component.h"
#include "ObjectDestructionManager.h"
#include "Transform.h"

void GOTOEngine::GameObject::InitInstance()
{
	m_scene = SceneManager::Get() ? SCENE_GET_CURRENTSCENE() : nullptr;
	m_scene->RegisterGameObject(this);

	//transform은 직접 생성 후 m_components에 등록
	m_transform = new Transform();
	m_transform->m_gameObject = this;
	m_transform->SetParent(nullptr); // 부모가 없으므로 nullptr로 설정

	RegisterComponent(m_transform); // Transform을 컴포넌트로 등록
}

void GOTOEngine::GameObject::RegisterComponent(Component* comp)
{
	m_components.push_back(comp);
}

void GOTOEngine::GameObject::UnregisterComponent(Component* comp)
{
	auto it = std::find(m_components.begin(), m_components.end(), comp);
	if (it != m_components.end()) {
		*it = std::move(m_components.back()); // 마지막 원소를 덮어씀
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
	////====== 트랜스 폼 파괴 ======////
	// 
	// 
	//트랜스폼의 차일드들도 일괄 파괴해야함...
	for (auto& child : m_transform->m_childs)
	{
		//iterator 오염 방지 -> RemoveChild()방지;
		child->m_parent = nullptr;
		Object::DestroyImmediate(child->GetGameObject()); // 즉시 파괴
	}
	m_transform->m_childs.clear(); // 자식 트랜스폼 목록 초기화

	if (m_transform->m_parent)
		m_transform->m_parent->RemoveChild(m_transform);

	delete m_transform; // Transform 컴포넌트 파괴
	m_transform = nullptr;
	//
	//
	////===========================////



	////====== 컴포넌트 파괴 =======////
	//
	//
	for (auto& comp : m_components)
	{
		if (!Object::IsValidObject(comp))
			continue; // 이미 파괴된 컴포넌트는 무시

		//iterator 오염 방지 -> UnregisterComponent()호출 방지
		comp->m_gameObject = nullptr;

		Object::DestroyImmediate(comp);
	}
	m_components.clear();
	//
	//
	////===========================////




	////====== 씬 등록 해제 ========////
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
