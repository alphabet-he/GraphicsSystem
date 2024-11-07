#include "cProceduralMeshBuilder.h"
#include "Engine/Platform/Platform.h"
#include <Tools/AssetBuildLibrary/Functions.h>
#include "External/JSON/Includes.h"
#include "stb_perlin.h"
#include <fstream>

eae6320::cResult eae6320::Assets::cProceduralMeshBuilder::Build(const std::vector<std::string>& i_arguments)
{
	float x_start, x_end, y_start, y_end; // the border of the terrain
	float step; // length of each grid
	uint8_t seed; // the stb perlin noise library always wrap at 256
	float lacunarity, gain, octaves; // fbm parameters
	float max_height, min_height; // normalize generated height


	// read json
	// TODO

	// generate fbm map
	uint16_t i_x_gridCnt, i_y_gridCnt;
	i_x_gridCnt = abs(x_start - x_end) / step;
	std::vector<std::vector<float>> height(width, std::vector<float>(height))
	return cResult();
}
