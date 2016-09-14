#pragma once

#include "Mesh.h"
#include "FBXManager.h"
#include "Texture.h"

class Skybox
{
	unsigned int texID;
	unsigned skyboxVAO, skyboxVBO;
	Texture texture[6];

public:
	Skybox();
	~Skybox();

	void LoadSkybox();
	void Render(unsigned int program, glm::mat4 view, glm::mat4 projection);


};

