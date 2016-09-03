#ifndef FBXMANAGER_H
#define FBXMANAGER_H

#include "FBXLoader.h"

class FBXManager {
	
private:

	FbxManager* manager;
	FbxIOSettings* io_settings;

public:

	FBXManager();
	~FBXManager();

	FbxScene * LoadFBX(const char * filepath);

};

#endif