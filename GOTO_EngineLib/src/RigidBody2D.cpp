#include "RigidBody2D.h"
#include "PhysicsManager.h"
#include "box2d-lite/Body.h"

GOTOEngine::RigidBody2D::RigidBody2D() : m_wrapperBody(nullptr), m_position({0,0}), m_rotation(0), m_mass(200.0f)
{
	PhysicsManager::Get()->RegisterRigidBody2D(this);
}

GOTOEngine::RigidBody2D::~RigidBody2D()
{
	PhysicsManager::Get()->UnRegisterRigigdBody2D(this);
}

void GOTOEngine::RigidBody2D::AddForce(Vector2 force)
{
	if(GetWrapperBody())
		m_wrapperBody->GetBody()->AddForce({ force.x, force.y });
}

GOTOEngine::Vector2 GOTOEngine::RigidBody2D::GetPosition()
{
	if (GetWrapperBody() && GetWrapperBody()->m_pBody)
		return Vector2{ m_wrapperBody->GetBody()->position.x,m_wrapperBody->GetBody()->position.y };

	else
		return m_position;
}

void GOTOEngine::RigidBody2D::SetPosition(Vector2 pos)
{
	if (GetWrapperBody() && GetWrapperBody()->m_pBody)
		m_wrapperBody->GetBody()->position = { pos.x, pos.y };
	else
		m_position = pos;
}

float GOTOEngine::RigidBody2D::GetRotation()
{
	if (GetWrapperBody() && GetWrapperBody()->m_pBody)
		return m_wrapperBody->GetBody()->rotation;
	else
		return m_rotation;
}

void GOTOEngine::RigidBody2D::SetRotation(float rot)
{
	if (GetWrapperBody() && GetWrapperBody()->m_pBody)
		m_wrapperBody->GetBody()->rotation = rot;
	else
		m_rotation = rot;
}

float GOTOEngine::RigidBody2D::GetMass()
{
	if (GetWrapperBody() && GetWrapperBody()->m_pBody)
		return m_wrapperBody->GetBody()->mass;
	else
		return m_mass;
}

void GOTOEngine::RigidBody2D::SetMass(float mass)
{
	if (GetWrapperBody() && GetWrapperBody()->m_pBody)
		m_wrapperBody->GetBody()->mass = mass;
	else
		m_mass = mass;
}


