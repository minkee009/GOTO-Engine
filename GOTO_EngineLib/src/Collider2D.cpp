#include "Collider2D.h"

GOTOEngine::Collider2D::Collider2D() 
{
	PhysicsManager::Get()->RegisterCollider2D(this);
}

GOTOEngine::Collider2D::~Collider2D()
{
	PhysicsManager::Get()->UnRegisterCollider2D(this);
}
