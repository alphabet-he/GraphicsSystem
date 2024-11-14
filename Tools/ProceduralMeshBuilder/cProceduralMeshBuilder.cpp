#include "cProceduralMeshBuilder.h"
#include "Engine/Platform/Platform.h"
#include <Tools/AssetBuildLibrary/Functions.h>
#include "External/JSON/Includes.h"
#include <fstream>
#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

eae6320::cResult eae6320::Assets::cProceduralMeshBuilder::Build(const std::vector<std::string>& i_arguments)
{
	auto result = eae6320::Results::Success;

	float i_x_start, i_x_end, i_y_start, i_y_end; // the border of the terrain
	float i_step; // length of each grid
	float i_seed; // the stb perlin noise library always wrap at 256
	float i_scale;
	float i_lacunarity, i_gain; uint8_t i_octaves; // fbm parameters
	float i_max_height, i_min_height; // normalize generated height

	std::vector<float> i_thresholds; // color division
	std::vector<std::vector<float>> i_colors; 

	// load file
	std::string i_pgsFilePath = m_path_source;
	eae6320::Platform::sDataFromFile dataFromFile;
	result = eae6320::Platform::LoadBinaryFile(i_pgsFilePath.c_str(), dataFromFile);
	if (!result) {
		eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "Can't load PGS data");
		return result;
	}

	// read json
	const auto parsedFile = nlohmann::json::parse(static_cast<const char*>(dataFromFile.data),
		static_cast<const char*>(dataFromFile.data) + dataFromFile.size);
	if (parsedFile.is_object()) {

		// x_border
		const auto x_border = parsedFile["x_border"];
		if (x_border.is_array()) {
			if (x_border[0].is_number_float() && x_border[1].is_number_float()) {
				i_x_start = x_border[0].get<float>();
				i_x_end = x_border[1].get<float>();
			}
			else {
				eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "x_border is not float.");
				result = eae6320::Results::Failure;
				return result;
			}
		}
		else {
			eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "x_border is not array.");
			result = eae6320::Results::Failure;
			return result;
		}

		// y_border
		const auto y_border = parsedFile["y_border"];
		if (y_border.is_array()) {
			if (y_border[0].is_number_float() && y_border[1].is_number_float()) {
				i_y_start = y_border[0].get<float>();
				i_y_end = y_border[1].get<float>();
			}
			else {
				eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "y_border is not float.");
				result = eae6320::Results::Failure;
				return result;
			}
		}
		else {
			eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "y_border is not array.");
			result = eae6320::Results::Failure;
			return result;
		}

		// step
		const auto step = parsedFile["step"];
		if (step.is_number_float()) {
			i_step = step.get<float>();
		}
		else {
			eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "step is not array.");
			result = eae6320::Results::Failure;
			return result;
		}

		// seed
		const auto seed = parsedFile["seed"];
		if (seed.is_number()) {
			i_seed = seed.get<float>();
			if (i_seed < 0) {
				// use random seed
				i_seed = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 256.0f));
				std::string output = "Using random seed: " + std::to_string(i_seed);
				eae6320::Assets::OutputWarningMessageWithFileInfo(i_pgsFilePath.c_str(), "Using random seed.");
			}
		}
		else {
			eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "seed is not number.");
			result = eae6320::Results::Failure;
			return result;
		}

		// scale
		const auto scale = parsedFile["scale"];
		if (scale.is_number_float()) {
			i_scale = scale.get<float>();
		}
		else {
			eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "scale is not float.");
			result = eae6320::Results::Failure;
			return result;
		}

		// lacunarity
		const auto lacunarity = parsedFile["lacunarity"];
		if (lacunarity.is_number_float()) {
			i_lacunarity = lacunarity.get<float>();
		}
		else {
			eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "lacunarity is not float.");
			result = eae6320::Results::Failure;
			return result;
		}

		// gain
		const auto gain = parsedFile["gain"];
		if (gain.is_number_float()) {
			i_gain = gain.get<float>();
			// Check gain is larger than 1
			if (i_gain <= 1.0f) {
				eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "gain must be greater than 1.");
				result = eae6320::Results::Failure;
				return result;
			}
		}
		else {
			eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "gain is not float.");
			result = eae6320::Results::Failure;
			return result;
		}

		// octaves
		const auto octaves = parsedFile["octaves"];
		if (octaves.is_number_integer()) {
			int temp_octaves = octaves.get<int>();
			// Check octaves is within the range of uint8_t
			if (temp_octaves < 0 || temp_octaves > 256) {
				eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "octaves is out of range for uint8_t.");
				result = eae6320::Results::Failure;
				return result;
			}
			i_octaves = static_cast<uint8_t>(temp_octaves);
		}
		else {
			eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "octaves is not integer.");
			result = eae6320::Results::Failure;
			return result;
		}

		// height_range
		const auto height_range = parsedFile["height_range"];
		if (height_range.is_array() && height_range.size() == 2) {
			if (height_range[0].is_number_float() && height_range[1].is_number_float()) {
				i_min_height = height_range[0].get<float>();
				i_max_height = height_range[1].get<float>();
			}
			else {
				eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "height_range values are not float.");
				result = eae6320::Results::Failure;
				return result;
			}
		}
		else {
			eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "height_range is not a valid array.");
			result = eae6320::Results::Failure;
			return result;
		}

		// thresholds
		const auto thresholds = parsedFile["thresholds"];
		if (thresholds.is_array()) {
			for (const auto& value : thresholds) {
				if (value.is_number_float()) {
					if (value >= i_max_height || value <= i_min_height) {
						eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "Thresholds exceeds height range");
						result = eae6320::Results::Failure;
						return result;
					}
					i_thresholds.push_back(value.get<float>());
				}
				else {
					eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "Thresholds are not float.");
					result = eae6320::Results::Failure;
					return result;
				}
			}
		}
		else {
			eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "thresholds is not an array.");
			result = eae6320::Results::Failure;
			return result;
		}

		// colors
		const auto colors = parsedFile["colors"];
		if (colors.is_array()) {
			for (const auto& color : colors) {
				if (color.is_array() && color.size() == 4 &&
					color[0].is_number_float() && color[1].is_number_float() &&
					color[2].is_number_float() && color[3].is_number_float()) {

					std::vector rgba = {
						color[0].get<float>(),
						color[1].get<float>(),
						color[2].get<float>(),
						color[3].get<float>()
					};
					i_colors.push_back(rgba);
				}
				else {
					eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "One of the colors is not an RGBA array.");
					result = eae6320::Results::Failure;
					return result;
				}
			}
			if (i_colors.size() != i_thresholds.size() + 1) {
				eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "Color and threshold counts do not match.");
				result = eae6320::Results::Failure;
				return result;
			}
		}
		else {
			eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "colors is not a valid array.");
			result = eae6320::Results::Failure;
			return result;
		}
	}
	else {
		eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "Data in file is not json data.");
		result = eae6320::Results::Failure;
		return result;
	}

	// check count limit
	if (abs(i_x_start - i_x_end) / i_step > 2048 || abs(i_y_start - i_y_end) / i_step > 2048) {
		eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "Point count exceeds limit");
		return eae6320::Results::Failure;
	}

	// initialize fbm map
	uint16_t i_x_gridCnt, i_y_gridCnt;
	i_x_gridCnt = static_cast<uint16_t>(std::floor(abs(i_x_start - i_x_end) / i_step)) + 1;
	i_y_gridCnt = static_cast<uint16_t>(std::floor(abs(i_y_start - i_y_end) / i_step)) + 1;
	
	std::vector<std::vector<float>> height(i_y_gridCnt, std::vector<float>(i_x_gridCnt));

	float min_value = std::numeric_limits<float>::max();
	float max_value = std::numeric_limits<float>::lowest();

	// generate fbm values
	for (int i = 0; i < i_y_gridCnt; i++) {
		for (int j = 0; j < i_x_gridCnt; j++) {
			height[i][j] = stb_perlin_fbm_noise3(
				i * i_scale + i_seed, j * i_scale + i_seed, 0, i_lacunarity, i_gain, i_octaves
			);
			if (height[i][j] < min_value) min_value = height[i][j];
			if (height[i][j] > max_value) max_value = height[i][j];
		}
	}

	// normalize height
	for (int i = 0; i < i_y_gridCnt; i++) {
		for (int j = 0; j < i_x_gridCnt; j++) {
			height[i][j] = (height[i][j] - min_value) / (max_value - min_value);
			height[i][j] = height[i][j] * (i_max_height - i_min_height) + i_min_height;
		}
	}

	// generate mesh files
	uint16_t i_split_y_gridCnt = static_cast<uint16_t>(floor(65535.0f / 6.0f / (i_x_gridCnt))) +1;
	uint8_t i_split_cnt = static_cast<uint8_t>(ceil(static_cast<float>(i_y_gridCnt) / static_cast<float>(i_split_y_gridCnt)));
	
	// get flatten index 
	auto GetFlattenIndex = [](uint16_t row_ind, uint16_t col_ind, uint16_t row_cnt) -> uint16_t {
		return row_ind * row_cnt + col_ind;
		};

	// generate output binary files
	for (uint8_t i = 0; i < i_split_cnt; i++) {

		// open file
		std::string suffixStr = "_" + std::to_string(i);
		std::string path_str = m_path_target;
		size_t dotPos = path_str.find_last_of('.');
		path_str = path_str.substr(0, dotPos) + suffixStr + path_str.substr(dotPos);

		std::ofstream outFile(path_str, std::ios::binary);
		if (!outFile.is_open()) {
			eae6320::Assets::OutputErrorMessageWithFileInfo(i_pgsFilePath.c_str(), "Failed to open file for writing.");
			return eae6320::Results::Failure;
		}

		// write data
		uint16_t i_vertexDataCount, i_indiceDataCount;

		// vertex count
		uint16_t i_curr_y_gridCnt = i_split_y_gridCnt;
		if (i == i_split_cnt - 1) { // the last one
			i_curr_y_gridCnt = i_y_gridCnt - i_split_y_gridCnt * i;
		}
		i_vertexDataCount = i_x_gridCnt * i_curr_y_gridCnt;
		outFile.write(reinterpret_cast<const char*>(&i_vertexDataCount), sizeof(uint16_t));

		// indice count
		i_indiceDataCount = (i_x_gridCnt - 1) * (i_curr_y_gridCnt - 1) * 6;
		outFile.write(reinterpret_cast<const char*>(&i_indiceDataCount), sizeof(uint16_t));

		// vertex data
		eae6320::Graphics::VertexFormats::sVertex_mesh* i_vertexData = new eae6320::Graphics::VertexFormats::sVertex_mesh[i_vertexDataCount];
		uint16_t curr_vertex = 0;
		for (uint16_t j = 0; j < i_curr_y_gridCnt; j++) {
			for (uint16_t k = 0; k < i_x_gridCnt; k++) {

				uint16_t row_ind = i_split_y_gridCnt * i + j;
				uint16_t col_ind = k;

				i_vertexData[curr_vertex].x = i_x_start + i_step * col_ind;
				i_vertexData[curr_vertex].y = height[row_ind][col_ind];
				i_vertexData[curr_vertex].z = i_y_start + i_step * row_ind;

				// check which color division the vertex is in
				bool b_flag = false;
				for (uint8_t i = 0; i < i_thresholds.size(); i++) {
					if (height[row_ind][col_ind] < i_thresholds[i]) {
						i_vertexData[curr_vertex].r = i_colors[i][0];
						i_vertexData[curr_vertex].g = i_colors[i][1];
						i_vertexData[curr_vertex].b = i_colors[i][2];
						i_vertexData[curr_vertex].a = i_colors[i][3];
						b_flag = true;
						break;
					}
				}
				if (!b_flag) {
					i_vertexData[curr_vertex].r = i_colors[i_thresholds.size()][0];
					i_vertexData[curr_vertex].g = i_colors[i_thresholds.size()][1];
					i_vertexData[curr_vertex].b = i_colors[i_thresholds.size()][2];
					i_vertexData[curr_vertex].a = i_colors[i_thresholds.size()][3];
				}

				curr_vertex++;
			}
		}
		outFile.write(reinterpret_cast<const char*>(i_vertexData),
			i_vertexDataCount * sizeof(eae6320::Graphics::VertexFormats::sVertex_mesh));

		// indice data
		uint16_t* i_indices = new uint16_t[i_indiceDataCount];
		uint16_t curr_index = 0;
		for (uint16_t j = 0; j < i_curr_y_gridCnt - 1; j++) {
			for (uint16_t k = 0; k < i_x_gridCnt - 1; k++) {

				uint16_t row_ind = static_cast<uint16_t>(i_split_y_gridCnt * i + j);
				uint16_t col_ind = static_cast<uint16_t>(k);
				// (0,0) -> (1,0) -> (0,1)
				// (1,0) -> (1,1) -> (0,1)
				i_indices[curr_index] = GetFlattenIndex(row_ind, col_ind, i_x_gridCnt);
				curr_index++;
				i_indices[curr_index] = GetFlattenIndex(row_ind+1, col_ind, i_x_gridCnt);
				curr_index++;
				i_indices[curr_index] = GetFlattenIndex(row_ind, col_ind+1, i_x_gridCnt);
				curr_index++;
				i_indices[curr_index] = GetFlattenIndex(row_ind+1, col_ind, i_x_gridCnt);
				curr_index++;
				i_indices[curr_index] = GetFlattenIndex(row_ind+1, col_ind+1, i_x_gridCnt);
				curr_index++;
				i_indices[curr_index] = GetFlattenIndex(row_ind, col_ind+1, i_x_gridCnt);
				curr_index++;
			}
		}
		outFile.write(reinterpret_cast<const char*>(i_indices), i_indiceDataCount * sizeof(uint16_t));

		outFile.close();
	}

	return result;
}
