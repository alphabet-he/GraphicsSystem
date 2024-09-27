#include "sGameObject.h"

eae6320::Assets::sGameObject::~sGameObject()
{
	if (m_Mesh)
	{
		m_Mesh->DecrementReferenceCount();
		m_Mesh = nullptr;
	}

	if (m_Effect)
	{
		m_Effect->DecrementReferenceCount();
		m_Effect = nullptr;
	}

	
}
