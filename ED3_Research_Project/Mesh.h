#ifndef MESH_H
#define MESH_H



#include <windows.h>

#ifdef APIENTRY

#undef APIENTRY
#endif 


#include <GL/glew.h>
#include "glm/glm.hpp"
#include <fbxsdk.h>
#include <vector>
#include <map>
#include "Animation.h"

class EntityManager;

class Mesh{

private:

	struct PackedVertex{
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 normal;
		glm::vec3 biTangent;
		bool operator<(const PackedVertex that) const{
			return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
		};
	};

	std::vector<int> controlPoints;
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;
	std::vector< glm::vec3> biTangents;

	std::vector<unsigned int> indices;
	std::vector<int> indexed_controlPoints;
	std::vector< glm::vec3 > indexed_vertices;
	std::vector< glm::vec2 > indexed_uvs;
	std::vector< glm::vec3 > indexed_normals;
	std::vector< glm::vec3> indexed_biTangents;


	unsigned int vertexArrayObject;
	unsigned int vertexBufferObject;
	unsigned int indexBufferID;
	Animation * myAnimation;
	std::vector<float> boneWeights;
	std::vector<int> boneIndicies;
	std::vector<glm::mat4> boneOffsets;
	int curAnim;
	int nextAnim;
	int curFrame;
	int nextFrame;

	bool isSecond;
	bool keyPress;


	float singleFrameTime;
	float CurTotalTime;
	float lastTime;

public:

	EntityManager * m_entityManager;
	Animation * myAnimations[2];

	~Mesh();

	bool LoadEntry(const char * path);
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

	unsigned int getVAO() { return vertexArrayObject; };

};

#endif