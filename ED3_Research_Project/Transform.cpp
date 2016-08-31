#include "Transform.h"
#include "tinyxml2.h"
#include "ResourceManager.h"

void Transform::LoadFromXML(tinyxml2::XMLElement * e){
	
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

Component * Transform::CreateInstance(){

	Transform * component = new Transform();

	return component;

}

Component * Transform::copy(){
	Transform * transform = new Transform(*this);

	return transform;
}