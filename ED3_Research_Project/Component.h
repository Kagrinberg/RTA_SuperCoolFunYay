#ifndef COMPONENT_H
#define COMPONENT_H

namespace tinyxml2 {
	class XMLElement;
}

class Component{

private:


public:

	virtual Component * copy() = 0 {};
	virtual void LoadFromXML(tinyxml2::XMLElement * e) = 0 {};

};


#endif