#include "Collider2D.h"

GOTOEngine::Collider2D::Collider2D() : m_wrapperBody(nullptr)
{
	
}

GOTOEngine::Collider2D::~Collider2D()
{
	PhysicsManager::Get()->UnRegisterCollider2D(this);
}

void GOTOEngine::Collider2D::AdditionalInitialize()
{
	PhysicsManager::Get()->RegisterCollider2D(this);
	if (!m_wrapperBody)
		DestroyImmediate(this);
}
