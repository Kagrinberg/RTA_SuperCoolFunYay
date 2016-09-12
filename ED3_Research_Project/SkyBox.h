#ifndef SKYBOX_H
#define SKYBOX_H

#include "TGA\Texture.h"

class SkyBox {

private:

	Texture m_skyboxTextures[6];

public:

	void LoadTextures(const char * path);

};

#endif