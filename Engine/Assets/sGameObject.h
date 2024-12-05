#ifndef EAE6320_ASSETS_SGAMEOBJECT_H
#define EAE6320_ASSETS_SGAMEOBJECT_H

#include "../Graphics/cEffect.h"
#include "../Graphics/cMesh.h"
#include "../Physics/sRigidBodyState.h"
#include "../CollisionSystem/BaseCollisionComponent.h"

namespace eae6320 {
	namespace Assets {

		struct sGameObject
		{
			Graphics::cMesh* m_Mesh = nullptr;
			Graphics::cEffect* m_Effect = nullptr;

			Physics::sRigidBodyState m_RigidBodyState;
			Collision::BaseCollisionComponent* m_collisionComp = nullptr;

			~sGameObject();
		};

	}
}

#endif

