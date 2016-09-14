#include "Texture.h"

#include <Windows.h>
#include <SOIL.h>
#include <GL\glew.h>
#include "GLError.h"


Texture::Texture(){

}

Texture::~Texture() {
}


void Texture::Load(const char * filename) {

	m_imageData = SOIL_load_image(filename, &m_width, &m_height, 0, SOIL_LOAD_RGB);

}

void Texture::LoadEntry(const char * filename)
{
	m_imageData = SOIL_load_image(filename, &m_width, &m_height, 0, SOIL_LOAD_RGB);

	glGenTextures(1, &m_textureID);
	check_gl_error();

	glBindTexture(GL_TEXTURE_2D, m_textureID);
	check_gl_error();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_imageData);
	check_gl_error();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	check_gl_error();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	check_gl_error();

	Free();


}

void Texture::Free() {

	SOIL_free_image_data(m_imageData);

}
