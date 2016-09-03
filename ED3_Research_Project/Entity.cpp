#include "Entity.h"

Entity::~Entity() {

	for (auto it = m_components.begin(); it != m_components.end(); it++) {
		delete it->second;
	}
}