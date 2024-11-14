--[[
	This file lists every asset that must be built by the AssetBuildSystem
]]

return
{
	shaders =
	{
		{ path = "Shaders/Vertex/standard.shader", arguments = { "vertex" } },
		{ path = "Shaders/Fragment/myShader.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/standard.shader", arguments = { "fragment" } },

		{ path = "Shaders/Vertex/vertexInputLayout_mesh.shader", arguments = { "vertex" } },
	},

	meshes = 
	{
		"Meshes/rectangle.json",
		"Meshes/triangle.json",
		"Meshes/plane.json",
		"Meshes/cone.json",
		"Meshes/torus.json",
		"Meshes/helix.json"
	},

	procedural = 
	{
		"PGS/terrain/test.json"
	}
}
