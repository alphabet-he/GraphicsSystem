#include "cMesh.h"

eae6320::Graphics::cMesh::cMesh()
{
}

eae6320::Graphics::cMesh::cMesh(unsigned int triangleCount,
	unsigned int vertexCountPerTriangle,
	eae6320::Graphics::VertexFormats::sVertex_mesh vertexData[])
{
	m_triangleCount = triangleCount;
	m_vertexCountPerTriangle = vertexCountPerTriangle;

	const unsigned int vertexCount = m_triangleCount * m_vertexCountPerTriangle;
	m_vertexData = new eae6320::Graphics::VertexFormats::sVertex_mesh[vertexCount];
	for (unsigned int i = 0; i < vertexCount; ++i)
	{
		m_vertexData[i] = vertexData[i];
	}
}

eae6320::Graphics::cMesh::~cMesh()
{
	delete[] m_vertexData;
	m_vertexData = nullptr;
}
