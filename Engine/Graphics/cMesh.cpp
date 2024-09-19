#include "cMesh.h"

eae6320::Graphics::cMesh::cMesh()
{
	m_triangleCount = 1;
	m_vertexCountPerTriangle = 3;

	m_vertexData = new eae6320::Graphics::VertexFormats::sVertex_mesh[3];
	{
		// clockwise
		m_vertexData[0].x = 0.0f;
		m_vertexData[0].y = 0.0f;
		m_vertexData[0].z = 0.0f;

		m_vertexData[1].x = 1.0f;
		m_vertexData[1].y = 0.0f;
		m_vertexData[1].z = 0.0f;

		m_vertexData[2].x = 1.0f;
		m_vertexData[2].y = 1.0f;
		m_vertexData[2].z = 0.0f;
	}

	m_indices = new uint16_t[3]{0, 1, 2};

}

eae6320::Graphics::cMesh::cMesh(unsigned int i_triangleCount,
	unsigned int i_vertexCountPerTriangle,
	eae6320::Graphics::VertexFormats::sVertex_mesh i_vertexData[],
	uint16_t i_indices[])
{
	m_triangleCount = i_triangleCount;
	m_vertexCountPerTriangle = i_vertexCountPerTriangle;

	const unsigned int vertexCount = m_triangleCount * m_vertexCountPerTriangle;
	m_vertexData = new eae6320::Graphics::VertexFormats::sVertex_mesh[vertexCount];
	for (unsigned int i = 0; i < vertexCount; ++i)
	{
		m_vertexData[i] = i_vertexData[i];
	}

	m_indices = new uint16_t[vertexCount];
	for (unsigned int i = 0; i < vertexCount; ++i)
	{
		m_indices[i] = i_indices[i];
	}
}

eae6320::Graphics::cMesh::~cMesh()
{
	delete[] m_vertexData;
	m_vertexData = nullptr;

	delete[] m_indices;
	m_indices = nullptr;
}
