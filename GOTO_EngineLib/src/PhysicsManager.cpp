#include "PhysicsManager.h"
#include "RigidBody2D.h"
#include "Collider2D.h"
#include "Mathf.h"

void GOTOEngine::PhysicsManager::RefreshBodyFromPhysicsWorld2D()
{
	if (m_needRefreshBodyInPhysicsWorld)
	{
		for (auto body : m_AddPendingBody)
		{
			m_physicsWorld2D->Add(body);
		}
		m_AddPendingBody.clear();

		for (auto body : m_removePendingBody)
		{
			m_physicsWorld2D->Remove(body);
		}
		m_removePendingBody.clear();
	}
}

void GOTOEngine::PhysicsManager::CheckAtiveBodyWrapper()
{
	for (auto pair : m_currentBody2Ds)
	{
		auto wrapperBody = pair.second;

		if (m_physicsWorld2D->IsValidBody(wrapperBody->GetBody())
			&& !wrapperBody->m_pOwner->IsActiveInHierarchy())
		{
			PendingRemoveBodyInWrapper(wrapperBody->GetBody());
		}

		else if (!m_physicsWorld2D->IsValidBody(wrapperBody->GetBody())
			&& wrapperBody->m_pOwner->IsActiveInHierarchy())
		{
			PendingAddBodyInWrapper(wrapperBody->GetBody());
		}
	}
}

void GOTOEngine::PhysicsManager::ApplyTransform()
{
	for (auto pair : m_currentBody2Ds)
	{
		auto wrapperBody = pair.second;

		if (m_physicsWorld2D->IsValidBody(wrapperBody->GetBody()))
		{
			auto transform = pair.second->GetTransform();

			transform->SetPosition({ wrapperBody->GetBody()->position.x,wrapperBody->GetBody()->position.y });
			transform->SetRotation(wrapperBody->GetBody()->rotation * Mathf::Rad2Deg);
		}
	}
}

void GOTOEngine::PhysicsManager::RegisterRigidBody2D(RigidBody2D* rigidBody)
{
	auto go = rigidBody->GetGameObject();
	auto it = m_currentBody2Ds.find(go);

	if (it != m_currentBody2Ds.end())
	{
		auto body2DWrapper = m_currentBody2Ds[go];

		//이미 중복 생성되어 있음
		if (body2DWrapper->HasRigidbody())
			return;

		body2DWrapper->InitRigidBody(rigidBody);
		rigidBody->m_wrapperBody = body2DWrapper;
		return;
	}

	auto createdBody2DWrapper = new Body2DWrapper(rigidBody->GetGameObject());
	createdBody2DWrapper->InitRigidBody(rigidBody);
	createdBody2DWrapper->m_pOwner = rigidBody->GetGameObject();
	PendingAddBodyInWrapper(createdBody2DWrapper->GetBody());
	rigidBody->m_wrapperBody = createdBody2DWrapper;

	m_currentBody2Ds[go] = createdBody2DWrapper;
}

void GOTOEngine::PhysicsManager::RegisterCollider2D(Collider2D* collider)
{
	auto go = collider->GetGameObject();
	auto it = m_currentBody2Ds.find(go);

	if (it != m_currentBody2Ds.end())
	{
		auto body2DWrapper = m_currentBody2Ds[go];

		//이미 중복 생성되어 있음
		if (body2DWrapper->HasCollider())
			return;

		body2DWrapper->InitCollider(collider);
		collider->m_wrapperBody = body2DWrapper;
		return;
	}

	auto createdBody2DWrapper = new Body2DWrapper(collider->GetGameObject());
	createdBody2DWrapper->InitCollider(collider);
	createdBody2DWrapper->m_pOwner = collider->GetGameObject();
	PendingAddBodyInWrapper(createdBody2DWrapper->GetBody());
	collider->m_wrapperBody = createdBody2DWrapper;

	m_currentBody2Ds[go] = createdBody2DWrapper;
}

void GOTOEngine::PhysicsManager::UnRegisterRigigdBody2D(RigidBody2D* rigidBody)
{
	auto go = rigidBody->GetGameObject();
	auto it = m_currentBody2Ds.find(go);

	if (it != m_currentBody2Ds.end())
	{
		auto body2DWrapper = m_currentBody2Ds[go];

		//이미 중복 파괴되어 있음
		if (!body2DWrapper->HasRigidbody())
			return;

		body2DWrapper->ExcludeRigidBody();

		//콜라이더도 없으면 아예 파괴
		if (!body2DWrapper->HasCollider())
		{
			m_currentBody2Ds.erase(it);
			PendingRemoveBodyInWrapper(body2DWrapper->GetBody());
			delete body2DWrapper;
		}
	}
}

void GOTOEngine::PhysicsManager::UnRegisterCollider2D(Collider2D* collider)
{
	auto go = collider->GetGameObject();
	auto it = m_currentBody2Ds.find(go);

	if (it != m_currentBody2Ds.end())
	{
		auto body2DWrapper = m_currentBody2Ds[go];

		//이미 중복 파괴되어 있음
		if (!body2DWrapper->HasRigidbody())
			return;

		body2DWrapper->ExcludeRigidBody();

		//강체도 없으면 아예 파괴
		if (!body2DWrapper->HasRigidbody())
		{
			m_currentBody2Ds.erase(it); 
			PendingRemoveBodyInWrapper(body2DWrapper->GetBody());
			delete body2DWrapper;
		}
	}
}

void GOTOEngine::PhysicsManager::Body2DWrapper::InitCollider(Collider2D* col)
{
	m_pBody->Set({50,50}, m_pBody->mass);
	m_pBody->position = { col->GetTransform()->GetPosition().x , col->GetTransform()->GetPosition().y };
	m_pCol = col;
}

void GOTOEngine::PhysicsManager::Body2DWrapper::InitRigidBody(RigidBody2D* rb)
{
	m_pBody->Set(m_pBody->width, 200.0f);
	m_pRb = rb;
}

void GOTOEngine::PhysicsManager::Body2DWrapper::ExcludeCollider()
{
	m_pBody->Set({ 0,0 }, m_pBody->mass);

	m_pCol = nullptr;
}

void GOTOEngine::PhysicsManager::Body2DWrapper::ExcludeRigidBody()
{
	m_pBody->Set(m_pBody->width, FLT_MAX);

	m_pRb = nullptr;
}
