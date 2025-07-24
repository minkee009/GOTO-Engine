#pragma once
#include "Singleton.h"
#include "box2d-lite/World.h"
#include "box2d-lite/Body.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include <unordered_map>

namespace GOTOEngine
{
	class PhysicsManager : public Singleton<PhysicsManager>
	{
	private:
		friend class Collider2D;
		friend class RigidBody2D;
		friend class Engine;
		class Body2DWrapper
		{
		private:
			friend class Collider2D;
			friend class RigidBody2D;
			friend class PhysicsManager;

			Body* m_pBody;		

			GameObject* m_pOwner;
			Collider2D* m_pCol;
			RigidBody2D* m_pRb;
		public:
			Body2DWrapper(GameObject* owner)
				: m_pBody(new Body())
				, m_pOwner(owner)
				, m_pCol(nullptr)
				, m_pRb(nullptr)
			{
				m_pBody->Set({ 0,0 }, FLT_MAX);
			}

			~Body2DWrapper()
			{
				if (m_pBody)
				{
					delete m_pBody;
				}
			}

			Body* GetBody() { return m_pBody; }
			
			Transform* GetTransform() { return m_pOwner->GetTransform(); }

			bool HasCollider() { return m_pCol; }
			bool HasRigidbody() { return m_pRb; }

			bool IsInPhysicsWorld() { return PhysicsManager::Get()->m_physicsWorld2D->IsValidBody(m_pBody); }

			void InitCollider(Collider2D* col);
			void InitRigidBody(RigidBody2D* rb);

			void ExcludeCollider();
			void ExcludeRigidBody();
		};

		

		int m_phyiscsIteration = 10;
		Vector2 m_gravity = { 0.0f, -50.0f };
		World* m_physicsWorld2D;

		bool m_needRefreshBodyInPhysicsWorld = false;    //RigidBody 등록 및 해제(파괴) 시 적용
		
		void RegisterRigidBody2D(RigidBody2D* rigidBody);
		void RegisterCollider2D(Collider2D* collider);

		void UnRegisterRigigdBody2D(RigidBody2D* rigidBody);
		void UnRegisterCollider2D(Collider2D* collider);

		void RefreshBodyFromPhysicsWorld2D();

		void PhysicsWorld2DRefreshDirtySet() { m_needRefreshBodyInPhysicsWorld = true; }

		void PendingAddBodyInWrapper(Body* body2DWrapper)
		{
			PhysicsWorld2DRefreshDirtySet();
			m_AddPendingBody.push_back(body2DWrapper);
		}

		void PendingRemoveBodyInWrapper(Body* body2DWrapper)
		{
			PhysicsWorld2DRefreshDirtySet();
			m_removePendingBody.push_back(body2DWrapper);
		}

		void CheckAtiveBodyWrapper();

		void MakeAndRegisterBodyWrapper2D();

		std::vector<Collider2D*> m_createdCollider2D;
		std::vector<RigidBody2D*> m_createdRigidBody2D;

		std::unordered_map<GameObject*, Body2DWrapper*> m_currentBody2Ds;

		std::vector<Body*> m_AddPendingBody;
		std::vector<Body*> m_removePendingBody;
	public:
		void StartUp()
		{
			m_physicsWorld2D = new World({ m_gravity.x, m_gravity.y }, m_phyiscsIteration);
		}

		Body2DWrapper* GetBody2DWrapper(GameObject* go)
		{
			auto it = m_currentBody2Ds.find(go);

			if (it != m_currentBody2Ds.end())
				return m_currentBody2Ds[go];

			return nullptr;
		}

		const std::unordered_map<GameObject*, Body2DWrapper*>& GetBody2DWrappers() const
		{
			return m_currentBody2Ds;
		}

		void Simulate(float deltaTime)
		{
			MakeAndRegisterBodyWrapper2D();

			RefreshBodyFromPhysicsWorld2D();

			if (m_physicsWorld2D)
				m_physicsWorld2D->Step(deltaTime);
		}

		void ApplyTransform();


		void ShutDown()
		{
			if (m_physicsWorld2D)
				delete m_physicsWorld2D;

			m_physicsWorld2D = nullptr;
		}
	};
}