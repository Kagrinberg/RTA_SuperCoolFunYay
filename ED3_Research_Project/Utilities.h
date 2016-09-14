#ifndef UTILITIES_H
#define UTILITIES_H

#ifdef APIENTRY

#undef APIENTRY
#endif 

#include <GL/glew.h>
#include <map>

#define BUFFER_OFFSET(i) ((char*)NULL + (i))
#define Stringification(i) #i


namespace Util
{

	unsigned int fnvHash(const char* str);

	const unsigned int FNV_PRIME = 16777619u;
    const unsigned int OFFSET_BASIS = 2166136261u;

	template<typename T>
	struct entry{
		T * resource;
		unsigned int refCount;
	};

	template<typename T>
	static unsigned int LoadEntry(const char * filePath, std::map<unsigned int, entry<T>> & map){
		unsigned int id = Util::fnvHash(filePath);
		std::map<unsigned int, entry<T>>::iterator it = map.find(id);
		//if entry doesnt exist add it.
		if(it == map.end()){
			T * temp = new T;
			temp->LoadEntry(filePath);
			map[id].resource = temp;
		}
		//Other wise increase the refrence count and return the id.
		map[id].refCount++;
		return id;
	}
	
	template<typename T>
	static unsigned int AddEntry(T * obj, std::map<unsigned int, entry<T>> & map){
		unsigned int id = Util::fnvHash(obj->getName());
		std::map<unsigned int, entry<T>>::iterator it = map.find(id);
		//if entry doesnt exist add it.
		if(it == map.end()){
			map[id].resource = obj;
		}
		map[id].refCount++;
		return id;
	}


}

#endif