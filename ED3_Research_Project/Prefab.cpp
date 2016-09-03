#include "Prefab.h"

Prefab::~Prefab() {

	for (auto it = m_components.begin(); it != m_components.end(); it++) {
		delete it->second;
	}

}

void Prefab::addComponent(unsigned int hash, Component * component){

	m_components[hash]= component;

}