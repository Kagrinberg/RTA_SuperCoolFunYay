#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <vector>
#include "Utilities.h"
#include "TGA\Texture.h"
#include "EntityManager.h"

class ResourceManager{

private:

	EntityManager * m_entityManager;

	static ResourceManager * s_instance;

	std::map<unsigned int, Util::entry<Mesh>> m_meshes;
	std::map<unsigned int, Util::entry<Texture>> m_textures;
	std::map<unsigned int, Util::entry<Material>> m_materials;

public:

	void setEntityManager(EntityManager * p_entityManager) { m_entityManager = p_entityManager; };

	static ResourceManager * getInstance()
	{
		if (!s_instance)
			s_instance = new ResourceManager;
		return s_instance;
	}

	ResourceManager();

	void LoadLevel(const char * p_name);
	void LoadResourcePack(const char * p_path);
	void LoadPrefab(const char * p_path);

	unsigned int LoadMesh(const char * p_model);
	unsigned int LoadTexture(const char * p_texture);
	unsigned int LoadMaterial(Material * p_material);

	Mesh * getMesh(unsigned int hashID) { return m_meshes[hashID].resource; };
	Material * getMaterial(unsigned int hashID) { return m_materials[hashID].resource; };
	Texture * getTexture(unsigned int hashID) { return m_textures[hashID].resource; };

	void Render();

};



#endif