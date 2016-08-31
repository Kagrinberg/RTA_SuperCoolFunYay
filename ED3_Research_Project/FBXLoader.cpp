#include <Windows.h>
#include "FBXLoader.h"
#include <set>

namespace FBXLoader
{


	////////////////////////////////////////////////////////////////////////////
	// LoadMesh:                    Load a mesh from an FbxMesh attribute.
	// [in]fbx_mesh:                The FbxMesh attribute to load data from.
	// [out]mesh:                   The mesh data loaded from the FbxMesh
	//                              attribute.
	// [in]fbx_joints:              The FBX representation of the hierarchy.
	// [out]control_point_indices:  The control point indices from which the
	//                              unique vertex originated.  This array and
	//                              the unique vertex array in the mesh would
	//                              be parallell containers.
	// return:                      True on success, false on failure.
	////////////////////////////////////////////////////////////////////////////
	bool LoadMesh(FbxMesh* fbx_mesh, Mesh& mesh,
		std::vector< unsigned int >& control_point_indices);


	////////////////////////////////////////////////////////////////////////////
	// LoadTexture:   Load the texture name from a mesh.
	// [in]fbx_mesh:  The FbxMesh attribute to load data from.
	// [out]mesh:     The mesh data loaded from the FbxMesh attribute.
	// return:        True on success, false on failure.
	////////////////////////////////////////////////////////////////////////////
	bool LoadTexture(FbxMesh* fbx_mesh, Mesh& mesh);

	//
	// End Forward declaration of internal methods used by FBXLoader::Load method
	////////////////////////////////////////////////////////////////////////////

	bool Load(const std::string &fileName,
		// [out] Test nmodels provided will only have one mesh, but other assets may have multiple
		// meshes using the same rig to create a model
		std::vector<Mesh > &meshes)
	{
		// Get an FBX manager
		FbxManager* manager = FbxManager::Create();
		if (manager == 0)
		{
			return false;
		}

		// Create IO settings
		FbxIOSettings* io_settings = FbxIOSettings::Create(manager, IOSROOT);
		if (io_settings == 0)
		{
			return false;
		}

		manager->SetIOSettings(io_settings);

		// Create importer
		FbxImporter* importer = FbxImporter::Create(manager, "");
		if (importer == 0)
		{
			return false;
		}

		// Initialize importer
		if (importer->Initialize(fileName.c_str(), -1, io_settings) == false)
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
		if (converter.SplitMeshesPerMaterial(scene, true) == false)
		{
			return false;
		}


		// Load Mesh data
		for (int i = 0; i < scene->GetSrcObjectCount< FbxMesh >(); ++i)
		{
			FbxMesh* mesh_attribute = scene->GetSrcObject< FbxMesh >(i);

			Mesh mesh;
			std::vector< unsigned int > control_point_indices;

			if (LoadMesh(mesh_attribute, mesh, control_point_indices) == false)
			{
				return false;
			}
			
			if (LoadTexture(mesh_attribute, mesh) == false)
			{
				return false;
			}

			meshes.push_back(mesh);
		}

		

		return true;
	}

	bool LoadMesh(FbxMesh* fbx_mesh, Mesh& mesh,
		std::vector< unsigned int >& control_point_indices)
	{
		return false;
		// TODO
		// Get control points - fbx_mesh->GetControlPoints()
		// For each polygon in mesh
			// For each vertex in current polygon
				// Get control point index - fbx_mesh->GetPolygonVertex(...)
				// Get Position of vertex
				// Get Texture Coordinates
				// Get Normals
				// Get any other needed mesh data, such as tangents
				// Iterate through unique vertices found so far...
				// if this vertex is unique add to set of unique vertices
				// Push index of where vertex lives in unique vertices container into index 
				// array, assuming you are using index arrays which you generally should be
			// End For each vertex in current polygon
		// End For each polygon in mesh

		//not working with control points yet
		FbxVector4 * allCPs = fbx_mesh->GetControlPoints();

		int polygonCount = fbx_mesh->GetPolygonCount();

		for (int i = 0; i < polygonCount; i++)
		{

		}







	}



	bool LoadTexture(FbxMesh* fbx_mesh, Mesh& mesh)
	{
		FbxProperty property;

		if (fbx_mesh->GetNode() == NULL)
			return false;

		int material_count = fbx_mesh->GetNode()->GetSrcObjectCount< FbxSurfaceMaterial >();

		for (int material_index = 0; material_index < material_count; ++material_index)
		{
			FbxSurfaceMaterial* surface_material = fbx_mesh->GetNode()->GetSrcObject< FbxSurfaceMaterial >(material_index);

			if (surface_material == 0)
				continue;

			int texture_index;

			FBXSDK_FOR_EACH_TEXTURE(texture_index)
			{
				property = surface_material->FindProperty(FbxLayerElement::sTextureChannelNames[texture_index]);

				if (property.IsValid() == false)
					continue;

				int texture_count = property.GetSrcObjectCount< FbxTexture >();

				for (int i = 0; i < texture_count; ++i)
				{
					// Ignore layered textures

					FbxTexture* texture = property.GetSrcObject< FbxTexture >(i);
					if (texture == 0)
						continue;

					FbxFileTexture* file_texture = FbxCast< FbxFileTexture >(texture);
					if (file_texture == 0)
						continue;

					std::string texture_name = file_texture->GetFileName();

					// TODO : something with the texture name here....

					//this gets just the texture name itself not the file address
					std::string::size_type pos = texture_name.find_last_of("/\\");
					if (pos != std::string::npos)
					{
						texture_name = texture_name.substr(pos + 1);
					}


					//add texture to the resource manager if it does not already exist
					Texture newTexture;

					newTexture.LoadEntry(texture_name.c_str());
					


				}
			}
		}

		return true;
	}


};
