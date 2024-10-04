#include "cMesh.h"
#include "Engine/Platform/Platform.h"
#include "External/JSON/Includes.h"

eae6320::cResult eae6320::Graphics::cMesh::Load(unsigned int i_triangleCount, unsigned int i_vertexCountPerTriangle, eae6320::Graphics::VertexFormats::sVertex_mesh i_vertexData[], uint16_t i_indices[], cMesh*& o_mesh)
{
	cMesh* newMesh = new cMesh(i_triangleCount, i_vertexCountPerTriangle, i_vertexData, i_indices);

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

	std::string i_meshPath = std::string("data/Meshes/") + i_meshDataFileName + std::string(".json");
	eae6320::Platform::sDataFromFile dataFromFile;
	result = eae6320::Platform::LoadBinaryFile(i_meshPath.c_str(), dataFromFile);
	if (!result) {
		EAE6320_ASSERTF(false, "Can't load mesh data");
		return result;
	}

	// Parse json data
	{
		const auto parsedFile = nlohmann::json::parse(static_cast<const char*>(dataFromFile.data),
			static_cast<const char*>(dataFromFile.data) + dataFromFile.size);
		if (parsedFile.is_object())
		{
			const auto triangle_count = parsedFile["triangle_count"];
			if (triangle_count.is_number()) {
				newMesh->m_triangleCount = triangle_count.get<int>();
			}
			else {
				EAE6320_ASSERTF(false, "triangle_count is not number.");
				result = eae6320::Results::Failure;
				return result;
			}

			const auto vertex_count_per_triangle = parsedFile["vertex_count_per_triangle"];
			if (vertex_count_per_triangle.is_number()) {
				newMesh->m_vertexCountPerTriangle = vertex_count_per_triangle.get<int>();
			}
			else {
				EAE6320_ASSERTF(false, "vertex_count_per_triangle is not number.");
				result = eae6320::Results::Failure;
				return result;
			}

			const auto vertex_data = parsedFile["vertex_data"];
			const auto indice_data = parsedFile["indice_data"];

			if (vertex_data.is_array() && indice_data.is_array()) {
				const auto i_vertexCount = newMesh->m_triangleCount * newMesh->m_vertexCountPerTriangle;
				newMesh->m_vertexData = new eae6320::Graphics::VertexFormats::sVertex_mesh[i_vertexCount];
				newMesh->m_indices = new uint16_t[i_vertexCount];

				for (unsigned int i = 0; i < i_vertexCount; i++)
				{
					const auto i_curr_vertex_data = vertex_data[i];
					if (i_curr_vertex_data.is_object())
					{
						const auto i_curr_vertex_position = i_curr_vertex_data["vertex_position"];
						if (i_curr_vertex_position.is_array()) {
							newMesh->m_vertexData[i].x = i_curr_vertex_position[0].get<float>();
							newMesh->m_vertexData[i].y = i_curr_vertex_position[1].get<float>();
							newMesh->m_vertexData[i].z = i_curr_vertex_position[2].get<float>();
						}
						else {
							EAE6320_ASSERTF(false, "vertex_position is not array.");
							result = eae6320::Results::Failure;
							return result;
						}
						// currently not deal with color
					}
					else {
						EAE6320_ASSERTF(false, "vertex_data is not object.");
						result = eae6320::Results::Failure;
						return result;
					}

					const auto i_curr_index_data = indice_data[i];
					if (i_curr_index_data.is_number()) {
						newMesh->m_indices[i] = i_curr_index_data.get<int>();
					}
					else {
						EAE6320_ASSERTF(false, "indice_data is not number.");
						result = eae6320::Results::Failure;
						return result;
					}
				}

			}
			else {
				EAE6320_ASSERTF(false, "vertex_data or indice_data is not array.");
				result = eae6320::Results::Failure;
				return result;
			}

		}

		else {
			EAE6320_ASSERTF(false, "Data in file is not json data.");
			result = eae6320::Results::Failure;
			return result;
		}
	}

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
	m_triangleCount = 0;
	m_vertexCountPerTriangle = 0;

	m_vertexData = nullptr;

	m_indices = nullptr;

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

	EAE6320_ASSERT(m_referenceCount == 0);
	const auto result = CleanUp();
	EAE6320_ASSERT(result);
}
