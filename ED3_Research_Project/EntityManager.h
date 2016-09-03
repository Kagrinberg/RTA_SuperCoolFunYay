#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <map>
#include "Entity.h"
#include "Prefab.h"
#include "Utilities.h"
#include "RenderingManager.h"

class EntityManager{

private:

	RenderingManager * m_renderingManager;

	std::map<unsigned int, Util::entry<Prefab>> m_prefabs;
	std::map<unsigned int, Util::entry<Entity>> m_entities;

public:

	~EntityManager();

	void setRenderingManager(RenderingManager * p_renderingManager) { m_renderingManager = p_renderingManager; };

	void addPrefab(Prefab * p_prefab);
	void addEntity(const char * p_entityName, const char * p_prefabName);

};

#endif