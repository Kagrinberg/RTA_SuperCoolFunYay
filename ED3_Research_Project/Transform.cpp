#include "Transform.h"
#include "tinyxml2.h"
#include "ResourceManager.h"
#include "glm/gtc/matrix_transform.hpp"


void Transform::LoadFromXML(tinyxml2::XMLElement * e, ResourceManager * p_resourceManager){
	
	tinyxml2::XMLElement * pElement = e->FirstChildElement("Position");
	tinyxml2::XMLElement * pChildElement = pElement->FirstChildElement();
	pChildElement->QueryFloatText(&m_vPosition.x);
	pChildElement = pChildElement->NextSiblingElement();
	pChildElement->QueryFloatText(&m_vPosition.y);
	pChildElement = pChildElement->NextSiblingElement();
	pChildElement->QueryFloatText(&m_vPosition.z);

	pElement = pElement->NextSiblingElement("Rotation");
	pChildElement = pElement->FirstChildElement();
	pChildElement->QueryFloatText(&m_vRotation.x);
	pChildElement = pChildElement->NextSiblingElement();
	pChildElement->QueryFloatText(&m_vRotation.y);
	pChildElement = pChildElement->NextSiblingElement();
	pChildElement->QueryFloatText(&m_vRotation.z);

	pElement = pElement->NextSiblingElement("Scale");
	pChildElement = pElement->FirstChildElement();
	pChildElement->QueryFloatText(&m_vScale.x);
	pChildElement = pChildElement->NextSiblingElement();
	pChildElement->QueryFloatText(&m_vScale.y);
	pChildElement = pChildElement->NextSiblingElement();
	pChildElement->QueryFloatText(&m_vScale.z);

}

Component * Transform::CopyComponent() {
	Transform * transform = new Transform(*this);
	return transform;
}


Component * Transform::CreateInstance(){
	Transform * component = new Transform();
	return component;
}


glm::mat4 * Transform::getMatrix() {

	m_Matrix = glm::mat4(1.0);

	
	glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(m_vRotation.x), glm::vec3(1, 0, 0));
	glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(m_vRotation.y), glm::vec3(0, 1, 0));
	glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(m_vRotation.z), glm::vec3(0, 0, 1));
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), m_vPosition);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_vScale);

	m_Matrix = trans * (rotX * rotY * rotZ) * scale;

	return &m_Matrix;
}
