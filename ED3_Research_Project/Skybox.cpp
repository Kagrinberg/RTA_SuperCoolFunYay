#include "Skybox.h"
#include <string>
using std::string;

Skybox::Skybox()
{
	m_mesh = new Mesh();
	m_FBX = new FBXManager();

	FbxScene *scene = m_FBX->LoadFBX("FBXOBJ\\Skybox.fbx");
	m_mesh->LoadMesh(scene);

}

Skybox::~Skybox()
{
	delete m_mesh;
	delete m_FBX;

}

void Skybox::LoadSkybox()
{
	//GLuint textureID;
	//glGenTextures(1, &textureID);
	//glActiveTexture(GL_TEXTURE0);
	//
	//int width, height;
	//unsigned char* image;
	//
	//string faces[6];
	//faces[0] = "Textures/Skybox/rt.tga";
	//faces[1] = "Textures/Skybox/lf.tga";
	//faces[2] = "Textures/Skybox/up.tga";
	//faces[3] = "Textures/Skybox/dn.tga";
	//faces[4] = "Textures/Skybox/bk.tga";
	//faces[5] = "Textures/Skybox/ft.tga";
	//
	//glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	//for (GLuint i = 0; i < 6; ++i)
	//{
	//	Texture texture;
	//	image = texture.LoadTexture(faces[i].c_str());
	//	glTexImage2D(
	//		GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
	//		GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
	//	);
	//}
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	//
	//return textureID;
}