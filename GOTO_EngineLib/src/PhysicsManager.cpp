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

void GOTOEngine::PhysicsManager::MakeAndRegisterBodyWrapper2D()
{
	for (auto col : m_createdCollider2D)
	{
		auto go = col->GetGameObject();
		auto it = m_currentBody2Ds.find(go);

		if (it != m_currentBody2Ds.end())
		{
			auto body2DWrapper = m_currentBody2Ds[go];

			//이미 중복 생성되어 있음
			if (body2DWrapper->HasCollider())
				continue;

			body2DWrapper->InitCollider(col);

			continue;
		}

		auto createdBody2DWrapper = new Body2DWrapper(col->GetGameObject());
		createdBody2DWrapper->InitCollider(col);
		createdBody2DWrapper->m_pOwner = col->GetGameObject();
		PendingAddBodyInWrapper(createdBody2DWrapper->GetBody());
		col->m_wrapperBody = createdBody2DWrapper;

		m_currentBody2Ds[go] = createdBody2DWrapper;
	}
	m_createdCollider2D.clear();

	for (auto rb : m_createdRigidBody2D)
	{
		auto go = rb->GetGameObject();
		auto it = m_currentBody2Ds.find(go);

		if (it != m_currentBody2Ds.end())
		{
			auto body2DWrapper = m_currentBody2Ds[go];

			//이미 중복 생성되어 있음
			if (body2DWrapper->HasRigidbody())
				continue;

			body2DWrapper->InitRigidBody(rb);

			continue;
		}

		auto createdBody2DWrapper = new Body2DWrapper(rb->GetGameObject());
		createdBody2DWrapper->InitRigidBody(rb);
		createdBody2DWrapper->m_pOwner = rb->GetGameObject();
		PendingAddBodyInWrapper(createdBody2DWrapper->GetBody());
		rb->m_wrapperBody = createdBody2DWrapper;

		m_currentBody2Ds[go] = createdBody2DWrapper;
	}
	m_createdRigidBody2D.clear();
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
	m_createdRigidBody2D.push_back(rigidBody);
}

void GOTOEngine::PhysicsManager::RegisterCollider2D(Collider2D* collider)
{
	m_createdCollider2D.push_back(collider);
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
	m_pBody->Set({col->m_size.x,col->m_size.y}, m_pBody->mass);
	m_pBody->position = { col->GetTransform()->GetPosition().x , col->GetTransform()->GetPosition().y };
}

void GOTOEngine::PhysicsManager::Body2DWrapper::InitRigidBody(RigidBody2D* rb)
{
	m_pBody->Set(m_pBody->width, rb->m_mass);
	if(rb->m_position.x != 0.0f
		|| rb->m_position.y != 0.0f)
	m_pBody->position = { rb->m_position.x , rb->m_position.y };
	if (rb->m_rotation != 0.0f)
		m_pBody->rotation = rb->m_rotation * Mathf::Deg2Rad;
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
