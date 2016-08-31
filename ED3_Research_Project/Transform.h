#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component.h"
#include "glm\glm.hpp"

class Transform : public Component{

private:

	glm::vec3 m_vPosition;
	glm::vec3 m_vRotation;
	glm::vec3 m_vScale;

	void LoadFromXML(tinyxml2::XMLElement * e);

public:

	Component * copy();
	static Component * CreateInstance();

};

#endif