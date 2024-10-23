#include "cMesh.h"
#include "Engine/Platform/Platform.h"
#include "External/JSON/Includes.h"

eae6320::cResult eae6320::Graphics::cMesh::Load(
	uint8_t i_vertexCountPerTriangle,
	uint16_t i_vertexDataCount,
	uint16_t i_indiceDataCount,
	eae6320::Graphics::VertexFormats::sVertex_mesh i_vertexData[], 
	uint16_t i_indices[],
	cMesh*& o_mesh)
{
	cMesh* newMesh = new cMesh(i_vertexCountPerTriangle, i_vertexDataCount, i_indiceDataCount, i_vertexData, i_indices);

	if (newMesh->InitializeGeometry())
	{
		EAE6320_ASSERT(newMesh != nullptr);
		o_mesh = newMesh;
		return Results::Success;
	}
	else
	{
		if (newMesh)
		{
			newMesh->DecrementReferenceCount();
			newMesh = nullptr;
		}
		o_mesh = nullptr;
		return Results::Failure;
	}
}

eae6320::cResult eae6320::Graphics::cMesh::Load(const char* i_meshDataFileName, cMesh*& o_mesh)
{
	auto result = eae6320::Results::Success;

	cMesh* newMesh = new cMesh();

	// Load binary data
	std::string i_meshPath = std::string("data/Meshes/") + i_meshDataFileName + std::string(".bin");
	eae6320::Platform::sDataFromFile dataFromFile;
	result = eae6320::Platform::LoadBinaryFile(i_meshPath.c_str(), dataFromFile);
	if (!result) {
		EAE6320_ASSERTF(false, "Can't load mesh data");
		return result;
	}

	newMesh->m_vertexCountPerTriangle = 3;

	auto currentOffset = reinterpret_cast<uintptr_t>(dataFromFile.data);
	const auto finalOffset = currentOffset + dataFromFile.size;

	newMesh->m_vertexDataCount = *reinterpret_cast<uint16_t*>(currentOffset);

	currentOffset += sizeof(uint16_t);
	newMesh->m_indiceDataCount = *reinterpret_cast<uint16_t*>(currentOffset);

	currentOffset += sizeof(uint16_t);
	newMesh->m_vertexData = new eae6320::Graphics::VertexFormats::sVertex_mesh[newMesh->m_vertexDataCount];
	std::memcpy(newMesh->m_vertexData, reinterpret_cast<VertexFormats::sVertex_mesh*>(currentOffset),
		newMesh->m_vertexDataCount * sizeof(VertexFormats::sVertex_mesh));

	currentOffset += newMesh->m_vertexDataCount * sizeof(VertexFormats::sVertex_mesh);
	newMesh->m_indices = new uint16_t[newMesh->m_indiceDataCount];
	std::memcpy(newMesh->m_indices, reinterpret_cast<uint16_t*>(currentOffset),
		newMesh->m_indiceDataCount * sizeof(uint16_t));

	dataFromFile.Free();

	// Initialization
	{
		result = newMesh->InitializeGeometry();

		if (result)
		{
			EAE6320_ASSERT(newMesh != nullptr);
			o_mesh = newMesh;
		}
		else
		{
			if (newMesh)
			{
				newMesh->DecrementReferenceCount();
				newMesh = nullptr;
			}
			o_mesh = nullptr;
			EAE6320_ASSERTF(false, "Can't initialize mesh.");
		}
	}

	return result;
}

eae6320::Graphics::cMesh::cMesh()
{
	m_vertexCountPerTriangle = 0;

	m_vertexDataCount = 0;
	m_indiceDataCount = 0;

	m_vertexData = nullptr;

	m_indices = nullptr;

}

eae6320::Graphics::cMesh::cMesh(
	uint8_t i_vertexCountPerTriangle,
	uint16_t i_vertexDataCount,
	uint16_t i_indiceDataCount,
	eae6320::Graphics::VertexFormats::sVertex_mesh i_vertexData[],
	uint16_t i_indices[])
{
	m_vertexCountPerTriangle = i_vertexCountPerTriangle;

	m_vertexDataCount = i_vertexCountPerTriangle;
	m_indiceDataCount = i_indiceDataCount;

	m_vertexData = new eae6320::Graphics::VertexFormats::sVertex_mesh[m_vertexDataCount];
	for (unsigned int i = 0; i < m_vertexDataCount; ++i)
	{
		m_vertexData[i] = i_vertexData[i];
	}

	m_indices = new uint16_t[m_indiceDataCount];
	for (unsigned int i = 0; i < m_indiceDataCount; ++i)
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

	EAE6320_ASSERT(m_referenceCount == 0);
	const auto result = CleanUp();
	EAE6320_ASSERT(result);
}
