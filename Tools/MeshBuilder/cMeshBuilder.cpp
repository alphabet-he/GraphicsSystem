#include "cMeshBuilder.h"
#include "Engine/Platform/Platform.h"
#include <Tools/AssetBuildLibrary/Functions.h>
#include "External/JSON/Includes.h"
#include <fstream>

eae6320::cResult eae6320::Assets::cMeshBuilder::Build(const std::vector<std::string>& i_arguments)
{
	auto result = eae6320::Results::Success;

	std::string i_meshPath = m_path_source;
	eae6320::Platform::sDataFromFile dataFromFile;
	result = eae6320::Platform::LoadBinaryFile(i_meshPath.c_str(), dataFromFile);
	if (!result) {
		eae6320::Assets::OutputErrorMessageWithFileInfo(i_meshPath.c_str(), "Can't load mesh data");
		return result;
	}

	std::ofstream outFile(m_path_target, std::ios::binary);
	if (!outFile.is_open()) {
		eae6320::Assets::OutputErrorMessageWithFileInfo(i_meshPath.c_str(), "Failed to open file for writing.");
		return eae6320::Results::Failure;
	}

	// Parse json data
	{
		const auto parsedFile = nlohmann::json::parse(static_cast<const char*>(dataFromFile.data),
			static_cast<const char*>(dataFromFile.data) + dataFromFile.size);
		if (parsedFile.is_object())
		{
			/*
			const auto vertex_count_per_triangle = parsedFile["vertex_count_per_triangle"];
			if (vertex_count_per_triangle.is_number()) {
				newMesh->m_vertexCountPerTriangle = vertex_count_per_triangle.get<int>();
			}
			else {
				EAE6320_ASSERTF(false, "vertex_count_per_triangle is not number.");
				result = eae6320::Results::Failure;
				return result;
			}
			*/
			uint16_t i_vertexDataCount, i_indiceDataCount;


			const auto vertex_data_count = parsedFile["vertex_data_count"];
			if (vertex_data_count.is_number()) {
				int vertexDataCount = vertex_data_count.get<int>();
				if (vertexDataCount >= 0 && vertexDataCount <= 65535) {
					i_vertexDataCount = static_cast<uint16_t>(vertexDataCount);
					outFile.write(reinterpret_cast<const char*>(&i_vertexDataCount), sizeof(uint16_t));
				}
				else {
					eae6320::Assets::OutputErrorMessageWithFileInfo(i_meshPath.c_str(), "vertex_data_count exceeds uint16 range.");
					result = eae6320::Results::Failure;
					return result;
				}
			}
			else {
				eae6320::Assets::OutputErrorMessageWithFileInfo(i_meshPath.c_str(), "vertex_data_count is not number.");
				result = eae6320::Results::Failure;
				return result;
			}

			const auto indice_data_count = parsedFile["indice_data_count"];
			if (indice_data_count.is_number()) {
				int indiceDataCount = indice_data_count.get<int>();
				if (indiceDataCount >= 0 && indiceDataCount <= 65535) {
					i_indiceDataCount = static_cast<uint16_t>(indiceDataCount);
					outFile.write(reinterpret_cast<const char*>(&i_indiceDataCount), sizeof(uint16_t));
				}
				else {
					eae6320::Assets::OutputErrorMessageWithFileInfo(i_meshPath.c_str(), "indice_data_count exceeds uint16 range.");
					result = eae6320::Results::Failure;
					return result;
				}
			}
			else {
				eae6320::Assets::OutputErrorMessageWithFileInfo(i_meshPath.c_str(), "indice_data_count is not number.");
				result = eae6320::Results::Failure;
				return result;
			}

			const auto vertex_data = parsedFile["vertex_data"];

			if (vertex_data.is_array()) {
				eae6320::Graphics::VertexFormats::sVertex_mesh* i_vertexData = new eae6320::Graphics::VertexFormats::sVertex_mesh[vertex_data_count];

				for (unsigned int i = 0; i < i_vertexDataCount; i++)
				{
					const auto i_curr_vertex_data = vertex_data[i];
					if (i_curr_vertex_data.is_object())
					{
						// position
						const auto i_curr_vertex_position = i_curr_vertex_data["vertex_position"];
						if (i_curr_vertex_position.is_array()) {
							i_vertexData[i].x = i_curr_vertex_position[0].get<float>();
							i_vertexData[i].y = i_curr_vertex_position[1].get<float>();
							i_vertexData[i].z = i_curr_vertex_position[2].get<float>();
						}
						else {
							eae6320::Assets::OutputErrorMessageWithFileInfo(i_meshPath.c_str(), "vertex_position is not array.");
							result = eae6320::Results::Failure;
							return result;
						}

						// color
						const auto i_curr_vertex_color = i_curr_vertex_data["vertex_color"];
						if (i_curr_vertex_color.is_array()) {
							i_vertexData[i].r = i_curr_vertex_color[0].get<float>();
							i_vertexData[i].g = i_curr_vertex_color[1].get<float>();
							i_vertexData[i].b = i_curr_vertex_color[2].get<float>();
							i_vertexData[i].a = i_curr_vertex_color[3].get<float>();
						}
						else {
							eae6320::Assets::OutputErrorMessageWithFileInfo(i_meshPath.c_str(), "vertex_color is not array.");
							result = eae6320::Results::Failure;
							return result;
						}
					}
					else {
						eae6320::Assets::OutputErrorMessageWithFileInfo(i_meshPath.c_str(), "vertex_data is not object.");
						result = eae6320::Results::Failure;
						return result;
					}
				}
				outFile.write(reinterpret_cast<const char*>(i_vertexData),
					i_vertexDataCount * sizeof(eae6320::Graphics::VertexFormats::sVertex_mesh));
			}
			else {
				eae6320::Assets::OutputErrorMessageWithFileInfo(i_meshPath.c_str(), "vertex_data is not array.");
				result = eae6320::Results::Failure;
				return result;
			}

			const auto indice_data = parsedFile["indice_data"];

			if (indice_data.is_array()) {
				uint16_t* i_indices = new uint16_t[indice_data_count];

				for (unsigned int i = 0; i < indice_data_count; i++)
				{
					const auto i_curr_index_data = indice_data[i];
					if (i_curr_index_data.is_number()) {
						int indiceData = i_curr_index_data.get<int>();
						if (indiceData >= 0 && indiceData <= 65535) {
							i_indices[i] = static_cast<uint16_t>(indiceData);
						}
						else {
							eae6320::Assets::OutputErrorMessageWithFileInfo(i_meshPath.c_str(), "indice data exceeds uint16 range.");
							result = eae6320::Results::Failure;
							return result;
						}
					}
					else {
						eae6320::Assets::OutputErrorMessageWithFileInfo(i_meshPath.c_str(), "indice_data is not number.");
						result = eae6320::Results::Failure;
						return result;
					}
				}
				outFile.write(reinterpret_cast<const char*>(i_indices), i_indiceDataCount * sizeof(uint16_t));
			}
			else {
				eae6320::Assets::OutputErrorMessageWithFileInfo(i_meshPath.c_str(), "indice_data is not array.");
				result = eae6320::Results::Failure;
				return result;
			}

		}

		else {
			eae6320::Assets::OutputErrorMessageWithFileInfo(i_meshPath.c_str(), "Data in file is not json data.");
			result = eae6320::Results::Failure;
			return result;
		}
	}
	outFile.close();
    return result;
}
