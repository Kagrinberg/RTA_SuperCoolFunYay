#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>									// Standard windows header
#include <stdio.h>										// Standard I/O header 
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <fbxsdk.h>
#include <string>

static const GLubyte uTGAcompare[12] = {0,0,2, 0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
static const GLubyte cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};	// Compressed TGA Header

typedef struct
{
	GLubyte Header[12];									// TGA File Header
} TGAHeader;

typedef struct
{
	GLubyte		header[6];								// First 6 Useful Bytes From The Header
	unsigned int		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
	unsigned int		imageSize;								// Used To Store The Image Size When Setting Aside Ram
	unsigned int		temp;									// Temporary Variable
	unsigned int		type;	
	unsigned int		Height;									//Height of Image
	unsigned int		Width;									//Width ofImage
	unsigned int		Bpp;									// Bits Per Pixel
} TGA;

class Texture{

private:

	GLubyte	* imageData;									// Image Data (Up To 32 Bits)
	unsigned int	bpp;											// Image Color Depth In Bits Per Pixel
	unsigned int	width;											// Image Width
	unsigned int	height;											// Image Height
	unsigned int	texID;											// Texture ID Used To Select A Texture
	unsigned int	type;											// Image Type (GL_RGB, GL_RGBA)
	TGAHeader tgaheader;									// TGA header
	TGA tga;												// TGA image data


public:

	Texture();
	~Texture();
	bool LoadUncompressedTGA(const char *, FILE *);	// Load an Uncompressed file
	bool LoadCompressedTGA(const char *, FILE *);		// Load a Compressed file
	bool LoadEntry(const char * filename);

	unsigned int getTexID() { return texID; };

	bool ProcessTextures(FbxMesh* fbx_mesh);

};




#endif