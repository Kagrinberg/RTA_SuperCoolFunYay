#include "ShaderManager.h"
#include "Utilities.h"
#include <fstream>
#include <iostream>

ShaderManager::~ShaderManager() {
	for (auto it = m_programs.begin(); it != m_programs.end(); it++) {
		glDeleteProgram(it->second);
	}
}

void ShaderManager::CreateProgram(const std::string& p_shaderName, const std::string& p_vertexShaderFilename, const std::string& p_fragmentShaderFilename){
		//read the shader files and save the code
	std::string vertex_shader_code = ReadShader(p_vertexShaderFilename);
	std::string fragment_shader_code = ReadShader(p_fragmentShaderFilename);

	unsigned int vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code, "vertex shader");
	unsigned int fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, "fragment shader");

	int  link_result = 0;
	//create the program handle, attatch the shaders and link it
	unsigned int program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);

	if (link_result == GL_FALSE){

		int info_log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
		std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
		return;
	}
	m_programs[Util::fnvHash(p_shaderName.c_str())] = program;
}

unsigned int ShaderManager::CreateShader(GLenum p_shaderType, const std::string& p_source, const std::string& p_shaderName){
	int compile_result = 0;

	unsigned int shader = glCreateShader(p_shaderType);
	const char *shader_code_ptr = p_source.c_str();
	const int   shader_code_size = p_source.size();

	glShaderSource(shader, 1, &shader_code_ptr, &shader_code_size);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

	if (compile_result == GL_FALSE){

		int info_log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> shader_log(info_log_length);
		glGetShaderInfoLog(shader, info_log_length, NULL, &shader_log[0]);
		std::cout << "ERROR compiling shader: " << p_shaderName.c_str() << std::endl << &shader_log[0] << std::endl;
	}

	return shader;
}

std::string ShaderManager::ReadShader(const std::string& p_filename){

	std::string shaderCode;
	std::ifstream file(p_filename, std::ios::in);

	if (!file.good()){
		std::cout << "Can't read file " << p_filename.c_str() << std::endl;
		std::terminate();
	}

	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(&shaderCode[0], shaderCode.size());
	file.close();

	return shaderCode;

}

unsigned int ShaderManager::GetShader(const std::string& shaderName) {

	return m_programs.at(Util::fnvHash(shaderName.c_str()));

}