#include "cMesh.h"
#include "Engine/Platform/Platform.h"
#include "External/JSON/Includes.h"

eae6320::cResult eae6320::Graphics::cMesh::Load(
	unsigned int i_vertexCountPerTriangle, 
	unsigned int i_vertexDataCount,
	unsigned int i_indiceDataCount, 
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

			const auto vertex_count_per_triangle = parsedFile["vertex_count_per_triangle"];
			if (vertex_count_per_triangle.is_number()) {
				newMesh->m_vertexCountPerTriangle = vertex_count_per_triangle.get<int>();
			}
			else {
				EAE6320_ASSERTF(false, "vertex_count_per_triangle is not number.");
				result = eae6320::Results::Failure;
				return result;
			}

			const auto vertex_data_count = parsedFile["vertex_data_count"];
			if (vertex_data_count.is_number()) {
				newMesh->m_vertexDataCount = vertex_data_count.get<int>();
			}
			else {
				EAE6320_ASSERTF(false, "vertex_data_count is not number.");
				result = eae6320::Results::Failure;
				return result;
			}

			const auto indice_data_count = parsedFile["indice_data_count"];
			if (indice_data_count.is_number()) {
				newMesh->m_indiceDataCount = indice_data_count.get<int>();
			}
			else {
				EAE6320_ASSERTF(false, "triangle_count is not number.");
				result = eae6320::Results::Failure;
				return result;
			}

			const auto vertex_data = parsedFile["vertex_data"];

			if (vertex_data.is_array()) {
				newMesh->m_vertexData = new eae6320::Graphics::VertexFormats::sVertex_mesh[vertex_data_count];

				for (unsigned int i = 0; i < vertex_data_count; i++)
				{
					const auto i_curr_vertex_data = vertex_data[i];
					if (i_curr_vertex_data.is_object())
					{
						// position
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

						// color
						const auto i_curr_vertex_color = i_curr_vertex_data["vertex_color"];
						if (i_curr_vertex_color.is_array()) {
							newMesh->m_vertexData[i].r = i_curr_vertex_color[0].get<float>();
							newMesh->m_vertexData[i].g = i_curr_vertex_color[1].get<float>();
							newMesh->m_vertexData[i].b = i_curr_vertex_color[2].get<float>();
							newMesh->m_vertexData[i].a = i_curr_vertex_color[3].get<float>();
						}
						else {
							EAE6320_ASSERTF(false, "vertex_color is not array.");
							result = eae6320::Results::Failure;
							return result;
						}
						
					}
					else {
						EAE6320_ASSERTF(false, "vertex_data is not object.");
						result = eae6320::Results::Failure;
						return result;
					}
				}

			}
			else {
				EAE6320_ASSERTF(false, "vertex_data is not array.");
				result = eae6320::Results::Failure;
				return result;
			}

			const auto indice_data = parsedFile["indice_data"];

			if (indice_data.is_array()) {
				newMesh->m_indices = new uint16_t[indice_data_count];

				for (unsigned int i = 0; i < indice_data_count; i++)
				{
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
				EAE6320_ASSERTF(false, "indice_data is not array.");
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
	m_vertexCountPerTriangle = 0;

	m_vertexDataCount = 0;
	m_indiceDataCount = 0;

	m_vertexData = nullptr;

	m_indices = nullptr;

}

eae6320::Graphics::cMesh::cMesh(
	unsigned int i_vertexCountPerTriangle,
	unsigned int i_vertexDataCount,
	unsigned int i_indiceDataCount,
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
