#ifndef EAE6320_ASSETS_SGAMEOBJECT_H
#define EAE6320_ASSETS_SGAMEOBJECT_H

#include "../Graphics/cEffect.h"
#include "../Graphics/cMesh.h"
#include "../Physics/sRigidBodyState.h"

namespace eae6320 {
	namespace Assets {

		struct sGameObject
		{
			Graphics::cMesh* m_Mesh = nullptr;
			Graphics::cEffect* m_Effect = nullptr;

			Physics::sRigidBodyState m_RigidBodyState;

			~sGameObject();
		};

	}
}

#endif

