#include "Renderable.h"
#include "tinyxml2.h"
#include "ResourceManager.h"

void Renderable::LoadFromXML(tinyxml2::XMLElement * e, ResourceManager * p_resourceManager){


	tinyxml2::XMLElement * pElement = e->FirstChildElement("Mesh");
	tinyxml2::XMLElement * pChildElement = pElement->FirstChildElement("Location");

	const char * aname = pChildElement->GetText();

	m_meshID = p_resourceManager->LoadMeshFBX(pChildElement->GetText());

	//m_meshID = p_resourceManager->LoadMesh(pChildElement->GetText());

	pElement = pElement->NextSiblingElement("Material");
	pChildElement = pElement->FirstChildElement("Name");
	const char * name = pChildElement->GetText();

	Material * _Material = p_resourceManager->LoadMaterial(name);

	if (_Material != nullptr) {
		pChildElement = pElement->FirstChildElement("Shininess");
		float Shininess;
		pChildElement->QueryFloatText(&Shininess);
		pChildElement = pChildElement->NextSiblingElement("Diffuse");
		const char * diffuseTexture = pChildElement->GetText();
		unsigned int diffuseID = p_resourceManager->LoadTexture(diffuseTexture);

		pChildElement = pChildElement->NextSiblingElement("Specular");
		const char * specularTexture = pChildElement->GetText();
		unsigned int specularID = p_resourceManager->LoadTexture(specularTexture);

		pChildElement = pChildElement->NextSiblingElement("Normal");
		if (pChildElement != nullptr) {
			const char * normalTexture = pChildElement->GetText();
			unsigned int normalID = p_resourceManager->LoadTexture(normalTexture);
			_Material->setNormalID(normalID);
		}
		else {
			_Material->setNormalID(diffuseID);
		}

		_Material->setName(name);
		_Material->setShininess(Shininess);
		_Material->setDiffuseID(diffuseID);
		_Material->setSpecularID(specularID);

	}

	m_materialID = Util::fnvHash(name);

}

Component * Renderable::CopyComponent(){
	Renderable * renderable = new Renderable(*this);
	return renderable;
}

Component * Renderable::CreateInstance() {
	Renderable * component = new Renderable();
	return component;
}