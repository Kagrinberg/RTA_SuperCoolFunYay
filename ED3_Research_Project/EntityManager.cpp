#include "EntityManager.h"
#include "Utilities.h"
#include <iostream>
#include <windows.h>
#include "rlutil.h"
#include "Renderable.h"
#include "Transform.h"

void EntityManager::addPrefab(Prefab * p){

	Util::AddEntry(p, m_prefabs);
	rlutil::setColor(rlutil::LIGHTGREEN);
	std::cout << "[Prefab Loaded] ";
	rlutil::setColor(rlutil::WHITE);
	printf("Name: %s | Components: %d\n", p->getName(), p->getComponents().size());

}

//does this have a refrence count?
void EntityManager::addEntity(const char * entityName, const char * prefabName){

	unsigned int hashEntity = Util::fnvHash(entityName);
	unsigned int hashPrefab = Util::fnvHash(prefabName);

	Entity * entity = new Entity;
	entity->setName(entityName);

	for(auto & current : m_prefabs[hashPrefab].resource->getComponents()){
		entity->m_components[current.first] = current.second->copy();
		Renderable * renderable = dynamic_cast<Renderable *>(entity->m_components[current.first]);
		if(renderable != nullptr){
			renderable->setParent(entity);
			m_renderingManager->addRenderable(renderable);
		}
		Transform * transform = dynamic_cast<Transform *>(entity->m_components[current.first]);
		if(transform != nullptr){
			entity->setTransform(transform);
		}
	}

	m_entities[hashEntity].resource = entity;
	
	rlutil::setColor(rlutil::LIGHTGREEN);
	std::cout << "[Entity Loaded] ";
	rlutil::setColor(rlutil::WHITE);
	printf("Prefab: %s | Name: %s | Components: %d\n", prefabName, entityName, entity->m_components.size());

}