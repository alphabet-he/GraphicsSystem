#ifndef EAE6320_ASSETS_SCAMERA_H
#define EAE6320_ASSETS_SCAMERA_H

#include <Engine/Physics/sRigidBodyState.h>

namespace eae6320 {
	namespace Graphics {
		struct sCamera {
			Math::sVector m_cameraPositionPredicted;
			float m_verticalFieldOfView_inRadians;
			float m_aspectRatio = 1.0f; // default resolution being 512:512
			float m_z_nearPlane;
			float m_z_farPlane;

			Physics::sRigidBodyState m_RigidBodyState; // includes position, orientation
		};
	}
}

#endif
