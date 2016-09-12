#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include "glm/glm.hpp"
#include <fbxsdk.h>
#include <vector>
#include <map>
#include "Animation.h"


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

	std::vector<int> controlPoints;
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;

	std::vector<unsigned int> indices;
	std::vector<int> indexed_controlPoints;
	std::vector< glm::vec3 > indexed_vertices;
	std::vector< glm::vec2 > indexed_uvs;
	std::vector< glm::vec3 > indexed_normals;

	unsigned int vertexArrayObject;
	unsigned int vertexBufferObject;
	unsigned int indexBufferID;
	Animation * myAnimation;
	std::vector<glm::mat4> boneOffsets;
	int curFrame;

public:

	~Mesh();

	bool LoadMesh(FbxScene* scene);

	void GenerateIndices();
	void GenerateBuffers();
	bool getSameVertexIndex(PackedVertex & packed, std::map<PackedVertex, unsigned int> & VertexToOutIndex, unsigned int & result);

	std::vector<glm::vec3> getVertices() { return indexed_vertices; };
	std::vector<glm::vec2> getUvs() { return indexed_uvs; };
	std::vector<glm::vec3> getNormals() { return indexed_normals; };
	std::vector<unsigned int> getIndices() { return indices; };
	void setActive();
	void setAnimator(Animation * theAnimator);
	bool isAnimated();

};

#endif