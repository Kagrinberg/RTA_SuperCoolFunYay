#ifndef LIGHTINGMANAGER_H
#define LIGHTINGMANAGER_H

#include "Light.h"

#define MAX_LIGHTS 256

class LightingManager {

private:

	LightingManager * m_lightingManager;

	Light * m_lights[MAX_LIGHTS];

	unsigned int m_numLights, m_numDirectionalLights, m_numPointLights, m_numSpotLights;

public:

	LightingManager();

	void addLight(Light * light);

	void ActivateLights(unsigned int program);

	void ActivateDirectionalLight(unsigned int program, unsigned int index);
	void ActivatePointLight(unsigned int program, unsigned int index);
	void ActivateSpotLight(unsigned int program, unsigned int index);


};

#endif