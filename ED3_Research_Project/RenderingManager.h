#ifndef RENDERINGMANAGER_H
#define RENDERINGMANAGER_H

#include "Renderable.h"

#define MAX_RENDERABLES 1024

class RenderingManager{

private:

	Renderable * m_renderables[MAX_RENDERABLES];
	unsigned int m_numRenderables;

public:

	RenderingManager();

	void RenderAll();
	void addRenderable(Renderable * p_renderable);

};

#endif