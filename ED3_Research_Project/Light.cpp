#include "Light.h"
#include "tinyxml2.h"
#include "ResourceManager.h"

Light::Light() {
	m_position = m_direction = m_ambient = m_diffuse = m_specular = glm::vec3(1.0f);

	m_constant = 1.0f;
	m_linear = 0.09f;
	m_quadratic = 0.032f;

	m_cutOff = glm::cos(glm::radians(12.5f));
	m_outerCutOff = glm::cos(glm::radians(15.0f));
}

void Light::LoadFromXML(tinyxml2::XMLElement * e, ResourceManager * p_resourceManager) {
	
	tinyxml2::XMLElement * pElement = e->FirstChildElement("Type");
	const char * type = pElement->GetText();

	if (!strcmp(type, "Directional")) {
		m_type = Type::DIRECTIONAL_LIGHT;
	}
	else if (!strcmp(type, "Point")) {
		m_type = Type::POINT_LIGHT;
	}
	else if (!strcmp(type, "Spot")) {
		m_type = Type::SPOT_LIGHT;
	}

	tinyxml2::XMLElement * pChildElement;
	pElement = pElement->NextSiblingElement("Position");
	pChildElement = pElement->FirstChildElement();
	pChildElement->QueryFloatText(&m_position.x);
	pChildElement = pChildElement->NextSiblingElement();
	pChildElement->QueryFloatText(&m_position.y);
	pChildElement = pChildElement->NextSiblingElement();
	pChildElement->QueryFloatText(&m_position.z);

	pElement = pElement->NextSiblingElement("Direction");
	pChildElement = pElement->FirstChildElement();
	pChildElement->QueryFloatText(&m_direction.x);
	pChildElement = pChildElement->NextSiblingElement();
	pChildElement->QueryFloatText(&m_direction.y);
	pChildElement = pChildElement->NextSiblingElement();
	pChildElement->QueryFloatText(&m_direction.z);

	pElement = pElement->NextSiblingElement("Ambient");
	pChildElement = pElement->FirstChildElement();
	pChildElement->QueryFloatText(&m_ambient.x);
	pChildElement = pChildElement->NextSiblingElement();
	pChildElement->QueryFloatText(&m_ambient.y);
	pChildElement = pChildElement->NextSiblingElement();
	pChildElement->QueryFloatText(&m_ambient.z);

	pElement = pElement->NextSiblingElement("Diffuse");
	pChildElement = pElement->FirstChildElement();
	pChildElement->QueryFloatText(&m_diffuse.x);
	pChildElement = pChildElement->NextSiblingElement();
	pChildElement->QueryFloatText(&m_diffuse.y);
	pChildElement = pChildElement->NextSiblingElement();
	pChildElement->QueryFloatText(&m_diffuse.z);

	pElement = pElement->NextSiblingElement("Specular");
	pChildElement = pElement->FirstChildElement();
	pChildElement->QueryFloatText(&m_specular.x);
	pChildElement = pChildElement->NextSiblingElement();
	pChildElement->QueryFloatText(&m_specular.y);
	pChildElement = pChildElement->NextSiblingElement();
	pChildElement->QueryFloatText(&m_specular.z);

	pElement = pElement->NextSiblingElement("Constant");
	pElement->QueryFloatText(&m_constant);

	pElement = pElement->NextSiblingElement("Linear");
	pElement->QueryFloatText(&m_linear);

	pElement = pElement->NextSiblingElement("Quadratic");
	pElement->QueryFloatText(&m_quadratic);

	pElement = pElement->NextSiblingElement("CutOff");
	pElement->QueryFloatText(&m_cutOff);
	m_cutOff = glm::cos(glm::radians(m_cutOff));

	pElement = pElement->NextSiblingElement("OuterCutOff");
	pElement->QueryFloatText(&m_outerCutOff);
	m_outerCutOff = glm::cos(glm::radians(m_outerCutOff));


}

Component * Light::CopyComponent() {
	Light * light = new Light(*this);
	return light;
}

Component * Light::CreateInstance() {
	Light * component = new Light();
	return component;
}