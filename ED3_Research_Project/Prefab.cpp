#include "Prefab.h"

void Prefab::addComponent(unsigned int hash, Component * component){

	m_components[hash]= component;

}