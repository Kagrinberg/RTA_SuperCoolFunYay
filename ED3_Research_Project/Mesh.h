#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include "glm/glm.hpp"
#include <vector>
#include <map>


class Mesh{

private:

	struct PackedVertex{
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 normal;
		bool operator<(const PackedVertex that) const{
			return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
		};
	};

	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;

	std::vector<unsigned int> indices;
	std::vector< glm::vec3 > indexed_vertices;
	std::vector< glm::vec2 > indexed_uvs;
	std::vector< glm::vec3 > indexed_normals;

	unsigned int vertexArrayObject;
	unsigned int vertexBufferObject;
	unsigned int indexBufferID;

public:

	bool LoadEntry(const char * path);
	void GenerateIndices();
	void GenerateBuffers();
	bool getSameVertexIndex(PackedVertex & packed, std::map<PackedVertex, unsigned int> & VertexToOutIndex, unsigned int & result);

	std::vector<glm::vec3> getVertices() { return indexed_vertices; };
	std::vector<glm::vec2> getUvs() { return indexed_uvs; };
	std::vector<glm::vec3> getNormals() { return indexed_normals; };
	std::vector<unsigned int> getIndices() { return indices; };
	void setActive();

};

#endif