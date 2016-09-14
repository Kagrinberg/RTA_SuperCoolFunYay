#include "Mesh.h"
#include "GLError.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"
#include "EntityManager.h"

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

bool Mesh::LoadEntry(const char * path) {

	return true;
}


Mesh::~Mesh() {

	delete myAnimation;

}

void Mesh::GenerateIndices(){
	std::map<PackedVertex, unsigned int> VertexToOutIndex;

	for(unsigned int i = 0; i < vertices.size(); i++){
		PackedVertex packed = {vertices[i], uvs[i], normals[i]};

		unsigned int index;
		bool found = getSameVertexIndex(packed, VertexToOutIndex, index);

		if(found){
			indices.push_back(index);
		}
		else{
			indexed_vertices.push_back(vertices[i]);
			indexed_uvs.push_back(uvs[i]);
			indexed_normals.push_back(normals[i]);
			indexed_controlPoints.push_back(controlPoints[i]);
			unsigned int newindex = (unsigned int)indexed_vertices.size() -1;
			indices.push_back(newindex);
			VertexToOutIndex[packed] = newindex;
		}

	}

	

}

void Mesh::GenerateBuffers(){

	//Create Array Object
	glGenVertexArrays(1, &vertexArrayObject);
	check_gl_error();

	glBindVertexArray(vertexArrayObject);
	check_gl_error();


	//Create Vertex Buffer
	glGenBuffers(1, &vertexBufferObject);
	check_gl_error();

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	check_gl_error();

	unsigned int vertices_size = indexed_vertices.size() * sizeof(float);

	if (myAnimation->isAnimated())
	{
		glBufferData(GL_ARRAY_BUFFER, 16 * vertices_size, NULL, GL_STATIC_DRAW);
		check_gl_error();
	}
	else {
		glBufferData(GL_ARRAY_BUFFER, 8 * vertices_size, NULL, GL_STATIC_DRAW);
		check_gl_error();
	}

	glBufferSubData(GL_ARRAY_BUFFER, 0, 3*vertices_size, &indexed_vertices[0]);
	check_gl_error();

	glBufferSubData(GL_ARRAY_BUFFER, 3*vertices_size, 3*vertices_size, &indexed_normals[0]);
	check_gl_error();

	glBufferSubData(GL_ARRAY_BUFFER, 6*vertices_size, 2*vertices_size, &indexed_uvs[0]);
	check_gl_error();

	//create joint buffers
	if (myAnimation->isAnimated())
	{
		std::unordered_map<unsigned int, CtrlPoint*> controlMap = myAnimation->getMap();

		std::vector<int> boneIndicies;
		std::vector<float> boneWeights;


		for (unsigned int i = 0; i < indexed_controlPoints.size(); i++)
		{
			CtrlPoint * temp = controlMap[indexed_controlPoints[i]];

			for (unsigned int j = 0; j < 4; j++)
			{
				boneIndicies.push_back(temp->jointIndex[j]);
				boneWeights.push_back(temp->jointWeights[j]);
			}
		}

		glBufferSubData(GL_ARRAY_BUFFER,  8 * vertices_size, 4 * vertices_size, &boneWeights[0]);
		check_gl_error();

		glBufferSubData(GL_ARRAY_BUFFER, 12 * vertices_size, 4 * vertices_size, &boneIndicies[0]);
		check_gl_error();


	}

	//Create Index Buffer
	glGenBuffers(1, &indexBufferID);
	check_gl_error();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	check_gl_error();

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0] , GL_STATIC_DRAW);
	check_gl_error();

	unsigned int normalOffset = 3 * vertices_size;
	unsigned int textureCoordOffset = 6 * vertices_size;

	unsigned int boneWeight = 8 * vertices_size;
	unsigned int boneIndex = 12 * vertices_size;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	check_gl_error();

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(textureCoordOffset));
	check_gl_error();

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(normalOffset));
	check_gl_error();

	if (myAnimation->isAnimated()) {
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(boneWeight));
		check_gl_error();

		glVertexAttribPointer(4, 4, GL_INT, GL_FALSE, 0, BUFFER_OFFSET(boneIndex));
		check_gl_error();

		glEnableVertexAttribArray(3);
		check_gl_error();

		glEnableVertexAttribArray(4);
		check_gl_error();

	}


	glEnableVertexAttribArray(0);
	check_gl_error();

	glEnableVertexAttribArray(1);
	check_gl_error();

	glEnableVertexAttribArray(2);
	check_gl_error();

}

bool Mesh::LoadMesh(FbxScene* scene)
{
	curFrame = 0;
	keyPress = false;
	for (int i = 0; i < scene->GetSrcObjectCount< FbxMesh >(); ++i)
	{
		FbxMesh* mesh = scene->GetSrcObject< FbxMesh >(i);

		FbxVector4 *verts = mesh->GetControlPoints();

		FbxArray<FbxVector4> fnormals;
		mesh->GetPolygonVertexNormals(fnormals);

		FbxArray<FbxVector2> fuvs;
		FbxStringList nameList;
		mesh->GetUVSetNames(nameList);
		mesh->GetPolygonVertexUVs(nameList.GetStringAt(0), fuvs);

		for (int polyCount = 0; polyCount < mesh->GetPolygonCount(); ++polyCount)
		{
			if (mesh->GetPolygonSize(polyCount) > 3) return false;
			for (int polyVertCounter = 0; polyVertCounter < 3; ++polyVertCounter)
			{
				int polyVertIndex = mesh->GetPolygonVertex(polyCount, polyVertCounter);
				FbxVector4 fbxVert = verts[polyVertIndex];

				glm::vec3 vert;
				vert.x = static_cast<float>(fbxVert[0]);
				vert.y = static_cast<float>(fbxVert[1]);
				vert.z = static_cast<float>(fbxVert[2]);
				vertices.push_back(vert);
				controlPoints.push_back(polyVertIndex);
			}
		}

		for (int i = 0; i < fnormals.Size(); i++) {

			glm::vec3 normal = glm::vec3(fnormals[i][0], fnormals[i][1], fnormals[i][2]);
			normals.push_back(normal);
		}

		for (int i = 0; i < fuvs.Size(); i++) {

			glm::vec2 uv = glm::vec2(fuvs[i][0], fuvs[i][1]);
			uvs.push_back(uv);
		}

	}

	GenerateIndices();
	return true;
}

bool Mesh::getSameVertexIndex(PackedVertex & packed, std::map<PackedVertex, unsigned int> & VertexToOutIndex, unsigned int & result){
	std::map<PackedVertex, unsigned int>::iterator it = VertexToOutIndex.find(packed);
	if(it == VertexToOutIndex.end()){
		return false;
	}else{
		result = it->second;
		return true;
	}
}

void Mesh::setActive(){
	glBindVertexArray(vertexArrayObject);
	check_gl_error();

	if (myAnimation->isAnimated())
	{
		if (GetAsyncKeyState(VK_RIGHT))
		{
			if (!keyPress)
			{
				curFrame++;
				keyPress = true;
			}
			
		}
		else if (GetAsyncKeyState(VK_LEFT))
		{
			if (!keyPress)
			{
				curFrame--;
				keyPress = true;
			}
		}
		else
		{
			keyPress = false;
		}

		if (curFrame > myAnimation->getAniLength()-1)
		{
			curFrame = 0;
		}
		else if (curFrame < 0)
		{
			curFrame = myAnimation->getAniLength() - 1;
		}

		boneOffsets.clear();

		Skeleton mySkele = myAnimation->getSkele();
		
		for (unsigned int k = 0; k < mySkele.mJoints.size(); k++)
		{

			FbxAMatrix BindposeInverse = mySkele.mJoints[k].mGlobalBindposeInverse;

			glm::mat4 tempBone;
			tempBone[0] = glm::vec4(BindposeInverse.mData[0][0], BindposeInverse.mData[0][1], BindposeInverse.mData[0][2], BindposeInverse.mData[0][3]);
			tempBone[1] = glm::vec4(BindposeInverse.mData[1][0], BindposeInverse.mData[1][1], BindposeInverse.mData[1][2], BindposeInverse.mData[1][3]);
			tempBone[2] = glm::vec4(BindposeInverse.mData[2][0], BindposeInverse.mData[2][1], BindposeInverse.mData[2][2], BindposeInverse.mData[2][3]);
			tempBone[3] = glm::vec4(BindposeInverse.mData[3][0], BindposeInverse.mData[3][1], BindposeInverse.mData[3][2], BindposeInverse.mData[3][3]);

			FbxAMatrix keyMat = mySkele.mJoints[k].mAnimation[curFrame]->mGlobalTransform;

			glm::mat4 tempKey;
			tempKey[0] = glm::vec4(keyMat.mData[0][0], keyMat.mData[0][1], keyMat.mData[0][2], keyMat.mData[0][3]);
			tempKey[1] = glm::vec4(keyMat.mData[1][0], keyMat.mData[1][1], keyMat.mData[1][2], keyMat.mData[1][3]);
			tempKey[2] = glm::vec4(keyMat.mData[2][0], keyMat.mData[2][1], keyMat.mData[2][2], keyMat.mData[2][3]);
			tempKey[3] = glm::vec4(keyMat.mData[3][0], keyMat.mData[3][1], keyMat.mData[3][2], keyMat.mData[3][3]);

			glm::mat4 finalOffset = tempKey * tempBone;

			boneOffsets.push_back(finalOffset);


			std::string uniqueName = "jointSphere";
			uniqueName.append(std::to_string(k));

			m_entityManager->findEntity(uniqueName.c_str())->getTransform()->setPosition(glm::vec3(keyMat.mData[3][0], keyMat.mData[3][1], keyMat.mData[3][2]));
		}


		glm::mat4 finalMats[4];

		for (unsigned int i = 0; i < 4; i++)
		{
			finalMats[i] = boneOffsets[i];
		}

		for (unsigned int j = 0; j < 4; j++)
		{
			std::string strBO = "BoneOffset[";
			strBO.append(std::to_string(j));
			strBO.append("]");
			unsigned int location = glGetUniformLocation(6, strBO.c_str());
			glUniformMatrix4fv(location, 1, GL_TRUE, glm::value_ptr(finalMats[j]));
		}



		/*for (unsigned int i = 0; i < boneOffsets.size(); i++) {
			std::string strBO = "BoneOffset[";
			strBO.append(std::to_string(i));
			strBO.append("]");
			unsigned int location = glGetUniformLocation(6, strBO.c_str());
			glm::mat4 matrix = boneOffsets[i];
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
			check_gl_error();

		}*/

		/*unsigned int location = glGetUniformLocation(6, "BoneOffset");
		
		glUniformMatrix4fv(location, boneOffsets.size(), GL_TRUE, glm::value_ptr(boneOffsets[0]));
		check_gl_error();*/
	}

}

void Mesh::setAnimator(Animation * theAnimator)
{
	myAnimation = theAnimator;
}

bool Mesh::isAnimated()
{
	return myAnimation->isAnimated();
}