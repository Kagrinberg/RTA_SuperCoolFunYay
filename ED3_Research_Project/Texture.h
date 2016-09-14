#ifndef __TEXTURE_H__
#define __TEXTURE_H__


class Texture{

private:

	unsigned int m_textureID;

	unsigned char * m_imageData;
	int m_width;
	int m_height;

public:

	Texture();
	~Texture();

	void Load(const char * filename);
	void Free();

	void LoadEntry(const char * filename);

	unsigned int getTextureID() { return m_textureID; };
	unsigned char * getImage() { return m_imageData; }
	unsigned int GetWidth() { return m_width; }
	unsigned int GetHeight() { return m_height; }


};




#endif