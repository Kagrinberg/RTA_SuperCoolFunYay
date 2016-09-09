#include "LightingManager.h"
#include "Transform.h"
#include "glm/gtc/type_ptr.hpp"
#include "GLError.h"
#include <Windows.h>


LightingManager::LightingManager() {

	m_numLights = m_numDirectionalLights = m_numPointLights = m_numSpotLights = 0;

}

void LightingManager::addLight(Light * light) {

	m_lights[m_numLights] = light;
	m_numLights++;

};

void LightingManager::ActivateLights() {

	for (unsigned int i = 0; i < m_numLights; i++) {
		Type type = m_lights[i]->getType();
		switch (type){

		case Type::DIRECTIONAL_LIGHT:
		{
			ActivateDirectionalLight(i);
		}
		break;

		case Type::POINT_LIGHT:
		{
			ActivatePointLight(i);
		}
		break;

		case Type::SPOT_LIGHT:
		{
			ActivateSpotLight(i);
		}
		break;

		}
	}

};


void LightingManager::ActivateDirectionalLight(unsigned int index) {
	std::string uniformName = "dirLights[";
	uniformName.append(std::to_string(m_numDirectionalLights));
	uniformName.append("].");

	std::string direction = uniformName;
	direction.append("direction");

	std::string ambient = uniformName;
	ambient.append("ambient");
	std::string diffuse = uniformName;
	diffuse.append("diffuse");
	std::string specular = uniformName;
	specular.append("specular");

	glm::vec3 v_direction = m_lights[index]->getDirection();

	glm::vec3 v_ambient = m_lights[index]->getAmbient();
	glm::vec3 v_diffuse = m_lights[index]->getDiffuse();
	glm::vec3 v_specular = m_lights[index]->getSpecular();

	unsigned int directionLocation = glGetUniformLocation(3, direction.c_str());
	check_gl_error();

	unsigned int ambientLocation = glGetUniformLocation(3, ambient.c_str());
	check_gl_error();

	unsigned int diffuseLocation = glGetUniformLocation(3, diffuse.c_str());
	check_gl_error();

	unsigned int specularLocation = glGetUniformLocation(3, specular.c_str());
	check_gl_error();


	glUniform3f(directionLocation, v_direction.x, v_direction.y, v_direction.z);
	check_gl_error();

	glUniform3f(ambientLocation, v_ambient.x, v_ambient.y, v_ambient.z);
	check_gl_error();

	glUniform3f(diffuseLocation, v_diffuse.x, v_diffuse.y, v_diffuse.z);
	check_gl_error();

	glUniform3f(specularLocation, v_specular.x, v_specular.y, v_specular.z);
	check_gl_error();


	m_numDirectionalLights++;
}

void LightingManager::ActivatePointLight(unsigned int index) {
	std::string uniformName = "pointLights[";
	uniformName.append(std::to_string(m_numPointLights));
	uniformName.append("].");

	std::string position = uniformName;
	position.append("position");

	std::string constant = uniformName;
	constant.append("constant");
	std::string linear = uniformName;
	linear.append("linear");
	std::string quadratic = uniformName;
	quadratic.append("quadratic");

	std::string ambient = uniformName;
	ambient.append("ambient");
	std::string diffuse = uniformName;
	diffuse.append("diffuse");
	std::string specular = uniformName;
	specular.append("specular");

	glm::vec3 v_position = m_lights[index]->getPosition();

	float v_constant = m_lights[index]->getConstant();
	float v_linear = m_lights[index]->getLinear();
	float v_quadratic = m_lights[index]->getQuadratic();

	glm::vec3 v_ambient = m_lights[index]->getAmbient();
	glm::vec3 v_diffuse = m_lights[index]->getDiffuse();
	glm::vec3 v_specular = m_lights[index]->getSpecular();

	glUniform3f(glGetUniformLocation(3, position.c_str()), v_position.x, v_position.y, v_position.z);
	check_gl_error();

	glUniform1f(glGetUniformLocation(3, constant.c_str()), v_constant);
	check_gl_error();

	glUniform1f(glGetUniformLocation(3, linear.c_str()), v_linear);
	check_gl_error();

	glUniform1f(glGetUniformLocation(3, quadratic.c_str()), v_quadratic);
	check_gl_error();

	glUniform3f(glGetUniformLocation(3, ambient.c_str()), v_ambient.x, v_ambient.y, v_ambient.z);
	check_gl_error();

	glUniform3f(glGetUniformLocation(3, diffuse.c_str()), v_diffuse.x, v_diffuse.y, v_diffuse.z);
	check_gl_error();

	glUniform3f(glGetUniformLocation(3, specular.c_str()), v_specular.x, v_specular.y, v_specular.z);
	check_gl_error();


	m_numPointLights++;

}

void LightingManager::ActivateSpotLight(unsigned int index) {
	std::string uniformName = "spotLights[";
	uniformName.append(std::to_string(m_numSpotLights));
	uniformName.append("].");

	std::string position = uniformName;
	position.append("position");
	std::string direction = uniformName;
	direction.append("direction");
	std::string cutOff = uniformName;
	cutOff.append("cutOff");
	std::string outerCutOff = uniformName;
	outerCutOff.append("outerCutOff");

	std::string constant = uniformName;
	constant.append("constant");
	std::string linear = uniformName;
	linear.append("linear");
	std::string quadratic = uniformName;
	quadratic.append("quadratic");

	std::string ambient = uniformName;
	ambient.append("ambient");
	std::string diffuse = uniformName;
	diffuse.append("diffuse");
	std::string specular = uniformName;
	specular.append("specular");

	glm::vec3 v_position = m_lights[index]->getPosition();
	glm::vec3 v_direction = m_lights[index]->getDirection();
	float v_cutOff = m_lights[index]->getCutOff();
	float v_outerCutOff = m_lights[index]->getOuterCutoff();

	float v_constant = m_lights[index]->getConstant();
	float v_linear = m_lights[index]->getLinear();
	float v_quadratic = m_lights[index]->getQuadratic();

	glm::vec3 v_ambient = m_lights[index]->getAmbient();
	glm::vec3 v_diffuse = m_lights[index]->getDiffuse();
	glm::vec3 v_specular = m_lights[index]->getSpecular();

	//glUniform3f(glGetUniformLocation(3, position.c_str()), v_position.x, v_position.y, v_position.z);
	//glUniform3f(glGetUniformLocation(3, direction.c_str()), v_direction.x, v_direction.y, v_direction.z);
	glUniform1f(glGetUniformLocation(3, cutOff.c_str()), v_cutOff);
	check_gl_error();

	glUniform1f(glGetUniformLocation(3, outerCutOff.c_str()), v_outerCutOff);
	check_gl_error();

	glUniform1f(glGetUniformLocation(3, constant.c_str()), v_constant);
	check_gl_error();

	glUniform1f(glGetUniformLocation(3, linear.c_str()), v_linear);
	check_gl_error();

	glUniform1f(glGetUniformLocation(3, quadratic.c_str()), v_quadratic);
	check_gl_error();

	glUniform3f(glGetUniformLocation(3, ambient.c_str()), v_ambient.x, v_ambient.y, v_ambient.z);
	check_gl_error();

	glUniform3f(glGetUniformLocation(3, diffuse.c_str()), v_diffuse.x, v_diffuse.y, v_diffuse.z);
	check_gl_error();

	glUniform3f(glGetUniformLocation(3, specular.c_str()), v_specular.x, v_specular.y, v_specular.z);
	check_gl_error();


	m_numSpotLights++;

}