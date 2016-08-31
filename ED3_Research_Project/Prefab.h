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

	//Accessors
	const char * getName() { return m_name.c_str(); };

	//Mutators
	void setName(std::string p_name) { m_name = p_name; };

	void addComponent(unsigned int p_hash, Component * p_component);
	std::unordered_map<unsigned int, Component*> getComponents() { return m_components; };

	//glm::vec3 getPosition() { return vec_Position; };
	//glm::vec3 getRotation() { return vec_Rotation; };
	//glm::vec3 getScale() { return vec_Scale; };

	//Mesh * getMesh() { return _Mesh; };
	//Material * getMaterial() { return _Material; };

	//void setPosition(glm::vec3 p) { vec_Position = p; };
	//void setRotation(glm::vec3 r) { vec_Rotation = r; };
	//void setScale(glm::vec3 s) { vec_Scale = s; };

	//void setMesh(Mesh * m) { _Mesh = m; };
	//void setMaterial(Material * m) { _Material = m; };

};

#endif