#pragma once
#include "Component.h"
#include "PhysicsManager.h"
#include "Vector2.h"

namespace GOTOEngine
{
	class PhysicsManager::Body2DWrapper;
	class RigidBody2D : public Component
	{
	private:
		PhysicsManager::Body2DWrapper* m_wrapperBody;

		~RigidBody2D();
	public:
		RigidBody2D();

		void AddForce(Vector2 force);

		Vector2 GetPosition();
		void SetPosition(Vector2 pos);
		float GetRotation();
		void SetRotation(float rot);

		float GetMass();
		void SetMass(float mass);
	};
}