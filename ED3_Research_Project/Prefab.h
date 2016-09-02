#ifndef PREFAB_H
#define PREFAB_H

#include "Mesh.h"
#include "Material.h"
#include "glm/glm.hpp"
#include "Component.h"
#include <unordered_map>

class Prefab{

private:

	std::string m_name;

	std::unordered_map<unsigned int, Component *> m_components;

public:

	~Prefab();

	//Accessors
	const char * getName() { return m_name.c_str(); };

	//Mutators
	void setName(std::string p_name) { m_name = p_name; };

	void addComponent(unsigned int p_hash, Component * p_component);
	std::unordered_map<unsigned int, Component*> getComponents() { return m_components; };


};

#endif