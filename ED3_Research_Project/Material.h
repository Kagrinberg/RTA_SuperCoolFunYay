#ifndef MATERIAL_H
#define MATERIAL_H

#include "glm/glm.hpp"
#include <string>

class Material{

private:

	std::string m_name;
	unsigned int m_diffuseID;
	unsigned int m_specularID;
	unsigned int m_normalID;
	float m_shininess;

public:

	void setDiffuseID(unsigned int p_diffuseID) { m_diffuseID = p_diffuseID; };
	void setSpecularID(unsigned int p_specularID) { m_specularID = p_specularID; };
	void setNormalID(unsigned int p_normal) { m_normalID = p_normal; };
	void setShininess(float p_shininess) { m_shininess = p_shininess; };
	void setName(std::string p_name) { m_name = p_name; };

	unsigned int getDiffuseID(){ return m_diffuseID; };
	unsigned int getSpecularID() { return m_specularID; };
	unsigned int getNormalID() { return m_normalID; };
	float getShininess() { return m_shininess; };
	const char * getName() { return m_name.c_str(); };


};

#endif