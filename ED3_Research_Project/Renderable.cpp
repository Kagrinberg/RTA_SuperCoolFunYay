#include "Renderable.h"
#include "tinyxml2.h"
#include "ResourceManager.h"

void Renderable::LoadFromXML(tinyxml2::XMLElement * e){

	tinyxml2::XMLElement * pElement = e->FirstChildElement("Mesh");
	tinyxml2::XMLElement * pChildElement = pElement->FirstChildElement("Location");

	m_meshID = ResourceManager::getInstance()->LoadMesh(pChildElement->GetText());

	Material * _Material = new Material;

	pElement = pElement->NextSiblingElement("Material");
	pChildElement = pElement->FirstChildElement("Name");
	const char * name = pChildElement->GetText();
	pChildElement = pElement->FirstChildElement("Shininess");
	float Shininess;
	pChildElement->QueryFloatText(&Shininess);
	pChildElement = pChildElement->NextSiblingElement("Diffuse");
	const char * diffuseTexture = pChildElement->GetText();
	pChildElement = pChildElement->NextSiblingElement("Specular");
	const char * specularTexture = pChildElement->GetText();

	unsigned int diffuseID = ResourceManager::getInstance()->LoadTexture(diffuseTexture);
	unsigned int specularID = ResourceManager::getInstance()->LoadTexture(specularTexture);


	_Material->setName(name);
	_Material->setShininess(Shininess);
	_Material->setDiffuseID(diffuseID);
	_Material->setSpecularID(specularID);

	m_materialID = ResourceManager::getInstance()->LoadMaterial(_Material);

}

Component * Renderable::CreateInstance(){

	Renderable * component = new Renderable();

	return component;

}

Component * Renderable::copy(){
	Renderable * renderable = new Renderable(*this);

	return renderable;
}