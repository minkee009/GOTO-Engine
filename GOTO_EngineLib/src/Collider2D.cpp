#include "Collider2D.h"

GOTOEngine::Collider2D::Collider2D() : m_wrapperBody(nullptr), m_size({50,50})
{
	PhysicsManager::Get()->RegisterCollider2D(this);
}

GOTOEngine::Collider2D::~Collider2D()
{
	PhysicsManager::Get()->UnRegisterCollider2D(this);
}
