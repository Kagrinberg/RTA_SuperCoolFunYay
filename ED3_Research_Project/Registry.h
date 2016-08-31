//Singleton
#ifndef REGISTRY_H
#define REGISTRY_H

#include "Utilities.h"
#include "Component.h"
#include <unordered_map>

#define stringize(x) #x
#define register_type(X) Registry::getInstance()->registry[Util::fnvHash( stringize(X))] = X::CreateInstance

class Registry
{

private:

	//Instance of itself
	static Registry * s_instance;

public:

	typedef Component * (*FuncPointer)(); 
	std::unordered_map<unsigned int, FuncPointer> registry;

	static Registry * getInstance()
	{
		if (!s_instance)
			s_instance = new Registry;
		return s_instance;
	}

	//void AddComponent(

};


#endif