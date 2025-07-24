#include "RigidBody2D.h"
#include "PhysicsManager.h"
#include "box2d-lite/Body.h"

GOTOEngine::RigidBody2D::RigidBody2D()
{
	m_wrapperBody = PhysicsManager::Get()->RegisterRigidBody2D(this);
	if (!m_wrapperBody)
		DestroyImmediate(this);
}

GOTOEngine::RigidBody2D::~RigidBody2D()
{
	m_wrapperBody = nullptr;
	PhysicsManager::Get()->UnRegisterRigigdBody2D(this);
}

void GOTOEngine::RigidBody2D::AddForce(Vector2 force)
{
	m_wrapperBody->GetBody()->AddForce({ force.x, force.y });
}

GOTOEngine::Vector2 GOTOEngine::RigidBody2D::GetPosition()
{
	return Vector2{ m_wrapperBody->GetBody()->position.x,m_wrapperBody->GetBody()->position.y };
}

void GOTOEngine::RigidBody2D::SetPosition(Vector2 pos)
{
	m_wrapperBody->GetBody()->position = {pos.x, pos.y};
}

float GOTOEngine::RigidBody2D::GetRotation()
{
	return m_wrapperBody->GetBody()->rotation;
}

void GOTOEngine::RigidBody2D::SetRotation(float rot)
{
	m_wrapperBody->GetBody()->rotation = rot;
}

float GOTOEngine::RigidBody2D::GetMass()
{
	return m_wrapperBody->GetBody()->mass;
}

void GOTOEngine::RigidBody2D::SetMass(float mass)
{
	m_wrapperBody->GetBody()->mass = mass;
}


