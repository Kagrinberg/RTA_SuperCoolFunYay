#include "ResourceManager.h"
#include "tinyxml2.h"
#include "Registry.h"
#include "EntityManager.h"

ResourceManager::ResourceManager(){
	m_fbxManager = new FBXManager();
}

ResourceManager::~ResourceManager() {
	delete m_fbxManager;

	for (auto it = m_meshes.begin(); it != m_meshes.end(); it++) {
		delete it->second.resource;
	}

	for (auto it = m_materials.begin(); it != m_materials.end(); it++) {
		delete it->second.resource;
	}

	for (auto it = m_textures.begin(); it != m_textures.end(); it++) {
		delete it->second.resource;
	}



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
			comp->LoadFromXML(pComponent, this);
			_Prefab->addComponent(hash, comp);
		}else{
			printf("Could not load component (%s) in the prefab (%s).", cp_componentName, prefab_Name.c_str());
		}
	

		pComponent = pComponent->NextSiblingElement();

	}

	m_entityManager->addPrefab(_Prefab);

}

unsigned int ResourceManager::LoadMesh(const char * mesh){
	return LoadEntry(mesh, m_meshes);
}

unsigned int ResourceManager::LoadMeshFBX(const char * mesh) {

	unsigned int id = Util::fnvHash(mesh);
	std::map<unsigned int, Util::entry<Mesh>>::iterator it = m_meshes.find(id);
	//if entry doesnt exist add it.
	if (it == m_meshes.end()) {
		FbxScene * scene = m_fbxManager->LoadFBX(mesh);
		Mesh * temp = new Mesh();
		temp->LoadMesh(scene);
		m_meshes[id].resource = temp;

		//get Skeleton and joints
		Animation * myAnimation = new Animation();
		//myAnimation->setMesh(temp);
		temp->setAnimator(myAnimation);
		myAnimation->setScene(scene);
		myAnimation->CreateSkeleton();
		if (myAnimation->isAnimated())
		{
			myAnimation->makeCpts();
			myAnimation->createWeights();
			myAnimation->checkControls();


			//push back joint spheres into m_meshes
			for (unsigned int i = 0; i < myAnimation->getSkele().mJoints.size(); i++)
			{
				std::string uniqueName = "jointSphere";
				uniqueName.append(std::to_string(i));
			
				m_entityManager->addEntity(uniqueName.c_str(), "FBXSphere");

				glm::vec3 jointPos;
				jointPos.x = static_cast<float>(myAnimation->getSkele().mJoints[i].mGlobalBindposeInverse.mData[3][0]);
				jointPos.y = static_cast<float>(myAnimation->getSkele().mJoints[i].mGlobalBindposeInverse.mData[3][1]);
				jointPos.z = static_cast<float>(myAnimation->getSkele().mJoints[i].mGlobalBindposeInverse.mData[3][2]);


				m_entityManager->findEntity(uniqueName.c_str())->getTransform()->setPosition(jointPos);
			}

		
		}
		
		temp->GenerateBuffers();
	}
	//Other wise increase the refrence count and return the id.
	m_meshes[id].refCount++;
	return id;
}


unsigned int ResourceManager::LoadTexture(const char * texture){
	return LoadEntry(texture, m_textures);
}

Material * ResourceManager::LoadMaterial(const char * material){

	unsigned int id = Util::fnvHash(material);
	std::map<unsigned int, Util::entry<Material>>::iterator it = m_materials.find(id);
	Material * _material = nullptr;
	if (it == m_materials.end()) {
		_material = new Material();
		_material->setName(material);
		m_materials[id].resource = _material;
	}
	m_materials[id].refCount++;
	return _material;

}
