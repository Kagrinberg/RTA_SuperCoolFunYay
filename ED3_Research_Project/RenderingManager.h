#ifndef RENDERINGMANAGER_H
#define RENDERINGMANAGER_H

#include "Renderable.h"
#include "ResourceManager.h"

#define MAX_RENDERABLES 1024

class RenderingManager{

private:

	ResourceManager * m_resourceManager;

	Renderable * m_renderables[MAX_RENDERABLES];
	unsigned int m_numRenderables;

	unsigned int m_currentMaterialID;

public:

	RenderingManager();

	void RenderAll();
	void addRenderable(Renderable * p_renderable);
	void setResourceManager(ResourceManager * p_resourceManager) { m_resourceManager = p_resourceManager; };

	void sortByMaterial();
	bool sortFunction(Renderable * i, Renderable * j);

};

#endif