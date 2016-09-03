#include "FBXManager.h"
#include <Windows.h>

FBXManager::FBXManager() {

	manager = FbxManager::Create();

	io_settings = FbxIOSettings::Create(manager, IOSROOT);

	manager->SetIOSettings(io_settings);

}


FBXManager::~FBXManager() {
	manager->Destroy();
}

FbxScene * FBXManager::LoadFBX(const char * filepath) {

	// Create importer
	FbxImporter* importer = FbxImporter::Create(manager, "");
	if (importer == 0)
	{
		return false;
	}

	// Initialize importer
	if (importer->Initialize(filepath, -1, io_settings) == false)
	{
		std::string error;
		error += "FbxImporter::Initialize failed:  ";
		error += importer->GetStatus().GetErrorString();
#ifdef UNICODE
		std::wstring wideError = std::wstring(error.begin(), error.end());
		MessageBox(NULL, wideError.c_str(), L"Error", MB_OK);
#else
		MessageBox(NULL, error.c_str(), "Error", MB_OK);
#endif
		return false;
	}

	// Create a scene
	FbxScene* scene = FbxScene::Create(manager, "myScene");
	if (scene == 0)
	{
		return false;
	}

	// Load the scene with contents from the file
	if (importer->Import(scene) == false)
	{
		return false;
	}

	// No longer need the importer
	importer->Destroy();

	// Make sure everything is triangulated
	FbxGeometryConverter converter(manager);
	if (converter.Triangulate(scene, true) == false)
	{
		return false;
	}

	// If multiple materials are applied to a single mesh, split the mesh into separate meshes
	//if (converter.SplitMeshesPerMaterial(scene, true) == false)
	//{
	//	return false;
	//}

	return scene;

}