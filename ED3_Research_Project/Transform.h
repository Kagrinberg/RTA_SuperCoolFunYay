#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component.h"
#include "glm\glm.hpp"

class Transform : public Component{

private:

	glm::mat4 m_Matrix;
	glm::vec3 m_vPosition;
	glm::vec3 m_vRotation;
	glm::vec3 m_vScale;

	void LoadFromXML(tinyxml2::XMLElement * e, ResourceManager * p_resourceManager);

public:

	Component * CopyComponent();
	static Component * CreateInstance();
	glm::mat4 * getMatrix();

};

#endif