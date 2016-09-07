#ifndef LIGHT_H
#define LIGHT_H

#include "Component.h"
#include "Entity.h"

enum Type
{
	DIRECTIONAL_LIGHT = 0,
	POINT_LIGHT = 1,
	SPOT_LIGHT = 2
};

class Light : public Component {

private:

	Type m_type;

	glm::vec3 m_position;
	glm::vec3 m_direction;

	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;

	float m_constant;
	float m_linear;
	float m_quadratic;

	float m_cutOff;
	float m_outerCutOff;

	void LoadFromXML(tinyxml2::XMLElement * e, ResourceManager * p_resourceManager);

public:

	Light();

	Component * CopyComponent();
	static Component * CreateInstance();

	Type getType() { return m_type; };
	glm::vec3 getPosition() { return m_position; };
	glm::vec3 getDirection() { return m_direction; };

	glm::vec3 getAmbient() { return m_ambient; };
	glm::vec3 getDiffuse() { return m_diffuse; };
	glm::vec3 getSpecular() { return m_specular; };

	float getConstant() { return m_constant; };
	float getLinear() { return m_linear; };
	float getQuadratic() { return m_quadratic; };

	float getCutOff() { return m_cutOff; };
	float getOuterCutoff() { return m_outerCutOff; };

};

#endif
