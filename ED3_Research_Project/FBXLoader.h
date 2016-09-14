#pragma once
#include <string>
#include <vector>
#include <fbxsdk.h>

#include "Mesh.h"
#include "Texture.h"

namespace FBXLoader
{
	// Loads an FBX file and returns a filled out mesh, transform hierarchy and animation
	bool Load(const std::string &fileName,
		// [out] Test nmodels provided will only have one mesh, but other assets may have multiple
		// meshes using the same rig to create a model
		std::vector<Mesh > &meshes);
};

