#pragma once

#include "Mesh.h"
#include "FBXManager.h"
#include "TGA\Texture.h"

class Skybox
{
	Mesh* m_mesh;
	FBXManager* m_FBX;
	GLuint texID;
public:
	Skybox();
	~Skybox();

	void LoadSkybox();
	GLuint GetTexID() { return texID; }
};

