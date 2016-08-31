#ifndef ENTITY_H
#define ENTITY_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "Component.h"
#include <unordered_map>

class Entity{

private:

	std::string m_name;
	Transform * m_transform;
	
public:

	std::unordered_map<unsigned int, Component *> m_components;

public:


	const char * getName() { return m_name.c_str(); };
	Transform * getTransform() { return m_transform; };

	void setName(std::string p_name) { m_name = p_name; };
	void setTransform(Transform * p_transform) { m_transform = p_transform; };

	//Component * getComponent(std::string p_componentName);

};

#endif