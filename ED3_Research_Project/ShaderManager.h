#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <unordered_map>

#ifdef APIENTRY

#undef APIENTRY
#endif 

#include "GL\glew.h"

class ShaderManager{
	
private:

	std::unordered_map<unsigned int, unsigned int> m_programs;

public:

	~ShaderManager();
	void CreateProgram(const std::string& p_shaderName, const std::string& p_vertexShaderFilename, const std::string& p_fragmentShaderFilename);
	unsigned int GetShader(const std::string&);

private:

	unsigned int CreateShader(GLenum p_shaderType, const std::string& p_source, const std::string& p_shaderName);
	std::string ReadShader(const std::string& p_filename);

};

#endif