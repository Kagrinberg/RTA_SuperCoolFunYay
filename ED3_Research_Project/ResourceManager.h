#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <vector>
#include "Utilities.h"
#include "TGA\Texture.h"
#include "Material.h"
#include "FBXManager.h"
#include "Animation.h"

class EntityManager;

class ResourceManager{

private:

	EntityManager * m_entityManager;

	FBXManager * m_fbxManager;

	std::map<unsigned int, Util::entry<Mesh>> m_meshes;
	std::map<unsigned int, Util::entry<Texture>> m_textures;
	std::map<unsigned int, Util::entry<Material>> m_materials;

public:


	ResourceManager();
	~ResourceManager();

	void LoadLevel(const char * p_name);
	void LoadPrefab(const char * p_path);
	void setEntityManager(EntityManager * p_entityManager) { m_entityManager = p_entityManager; };


	unsigned int LoadMesh(const char * p_model);
	unsigned int LoadMeshFBX(const char * p_mesh);
	unsigned int LoadTexture(const char * p_texture);
	Material * LoadMaterial(const char * material);

	Mesh * getMesh(unsigned int hashID) { return m_meshes[hashID].resource; };
	Material * getMaterial(unsigned int hashID) { return m_materials[hashID].resource; };
	Texture * getTexture(unsigned int hashID) { return m_textures[hashID].resource; };

	FBXManager * getFBXManager() { return m_fbxManager; }

};



#endif