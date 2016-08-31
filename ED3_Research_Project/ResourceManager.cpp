#include "ResourceManager.h"
#include "tinyxml2.h"
#include "Registry.h"

ResourceManager * ResourceManager::s_instance = nullptr;

ResourceManager::ResourceManager(){
	
}

void ResourceManager::LoadLevel(const char * name){

	std::string filePath = "XML/Levels/";
	filePath.append(name);
	filePath.append(".xml");

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError eResult = doc.LoadFile( "XML/Levels/Level_1.xml" );

	tinyxml2::XMLNode * pLevel = doc.FirstChild();

	tinyxml2::XMLElement * pEntities = pLevel->FirstChildElement("Entities");

	tinyxml2::XMLElement * pEntity = pEntities->FirstChildElement("Entity");


	while(pEntity != nullptr){

		const char * cp_entityName = pEntity->FirstChildElement("Name")->GetText();
		const char * cp_prefab = pEntity->FirstChildElement("Prefab")->GetText();

		LoadPrefab(cp_prefab);

		m_entityManager->addEntity(cp_entityName, cp_prefab);

		pEntity = pEntity->NextSiblingElement();

	}

}

void ResourceManager::LoadPrefab(const char * path){

	Prefab * _Prefab = new Prefab();

	std::string filePath = "XML/Resources/Prefabs/";
	filePath.append(path);
	filePath.append(".xml");

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError eResult = doc.LoadFile(filePath.c_str());

	tinyxml2::XMLNode * pRoot = doc.FirstChild();

	tinyxml2::XMLElement * pPrefab = pRoot->FirstChildElement("Name");

	std::string prefab_Name = pPrefab->GetText();

	_Prefab->setName(prefab_Name.c_str());

	tinyxml2::XMLElement * pComponents = pPrefab->NextSiblingElement("Components");

	tinyxml2::XMLElement * pComponent = pComponents->FirstChildElement();

	while(pComponent != nullptr){

		const char * cp_componentName = pComponent->Name();

		unsigned int hash = Util::fnvHash(cp_componentName);

		Registry::FuncPointer functionPointer = Registry::getInstance()->registry[hash];

		if(functionPointer != nullptr){
			Component * comp = functionPointer();
			comp->LoadFromXML(pComponent);
			_Prefab->addComponent(hash, comp);
		}else{
			printf("Could not load component (%s) in the prefab (%s).", cp_componentName, prefab_Name);
		}
	

		pComponent = pComponent->NextSiblingElement();

	}

	m_entityManager->addPrefab(_Prefab);

}

unsigned int ResourceManager::LoadMesh(const char * mesh){
	return LoadEntry(mesh, m_meshes);
}

unsigned int ResourceManager::LoadTexture(const char * texture){
	return LoadEntry(texture, m_textures);
}

unsigned int ResourceManager::LoadMaterial(Material * material){
	return AddEntry(material, m_materials);
}
