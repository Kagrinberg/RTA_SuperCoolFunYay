#ifndef RENDERABLE_H
#define RENDERABLE_H



#include "Component.h"
#include "Mesh.h"
#include "TGA/Texture.h"
#include "Material.h"
#include "Entity.h"
#include "FBXManager.h"
#include "ResourceManager.h"

class Renderable : public Component{

private:

	Entity * m_parent;

	unsigned int m_meshID;
	unsigned int m_materialID;

	void LoadFromXML(tinyxml2::XMLElement * e, ResourceManager * p_resourceManager);

public:

	Component * copy();
	static Component * CreateInstance();
	void setParent(Entity * p_Entity){ m_parent = p_Entity; };

	unsigned int getMeshID() { return m_meshID; };
	unsigned int getMaterialID() { return m_materialID; };
	Entity * getParent() { return m_parent; };

};

#endif