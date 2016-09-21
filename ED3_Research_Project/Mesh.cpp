#include "Mesh.h"
#include "GLError.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"
#include "EntityManager.h"
#include "glm/gtx/compatibility.hpp"
#include <GLFW\glfw3.h>

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

bool Mesh::LoadEntry(const char * path) {

	return true;
}


Mesh::~Mesh() {

	delete myAnimations[0];

	if (myAnimations[1])
	{
		delete myAnimations[1];
	}

}

void Mesh::GenerateIndices() {
	std::map<PackedVertex, unsigned int> VertexToOutIndex;

	for (unsigned int i = 0; i < vertices.size(); i++) {
		PackedVertex packed;
		if (biTangents.size() != 0) {
			packed = { vertices[i], uvs[i], normals[i], biTangents[i] };
		}
		else {
			packed = { vertices[i], uvs[i], normals[i], normals[i] };
		}

		unsigned int index;
		bool found = getSameVertexIndex(packed, VertexToOutIndex, index);

		if (found) {
			indices.push_back(index);
		}
		else {
			indexed_vertices.push_back(vertices[i]);
			indexed_uvs.push_back(uvs[i]);
			indexed_normals.push_back(normals[i]);
			indexed_controlPoints.push_back(controlPoints[i]);
			if (controlPoints.size() == biTangents.size()) {
				indexed_biTangents.push_back(biTangents[i]);
			}
			unsigned int newindex = (unsigned int)indexed_vertices.size() - 1;
			indices.push_back(newindex);
			VertexToOutIndex[packed] = newindex;
		}

	}



}

void Mesh::GenerateBuffers() {

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

	//if (myAnimation->isAnimated())
	//{
	//	glBufferData(GL_ARRAY_BUFFER, 16 * vertices_size, NULL, GL_STATIC_DRAW);
	//	check_gl_error();
	//}
	//else {
	glBufferData(GL_ARRAY_BUFFER, 11 * vertices_size, NULL, GL_STATIC_DRAW);
	check_gl_error();
	//}

	glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * vertices_size, &indexed_vertices[0]);
	check_gl_error();

	glBufferSubData(GL_ARRAY_BUFFER, 3 * vertices_size, 3 * vertices_size, &indexed_normals[0]);
	check_gl_error();

	glBufferSubData(GL_ARRAY_BUFFER, 6 * vertices_size, 2 * vertices_size, &indexed_uvs[0]);
	check_gl_error();

	if (indexed_biTangents.size() != 0) {
		glBufferSubData(GL_ARRAY_BUFFER, 9 * vertices_size, 3 * vertices_size, &indexed_biTangents[0]);
		check_gl_error();
	}
	else {
		glBufferSubData(GL_ARRAY_BUFFER, 9 * vertices_size, 3 * vertices_size, &indexed_normals[0]);
		check_gl_error();
	}

	//create joint buffers
	if (myAnimation->isAnimated())
	{
		std::unordered_map<unsigned int, CtrlPoint*> controlMap = myAnimation->getMap();




		for (unsigned int i = 0; i < indexed_controlPoints.size(); i++)
		{
			CtrlPoint * temp = controlMap[indexed_controlPoints[i]];

			for (unsigned int j = 0; j < 4; j++)
			{
				boneWeights.push_back(temp->jointWeights[j]);
				boneIndicies.push_back(temp->jointIndex[j]);
			}
		}

		//glBufferSubData(GL_ARRAY_BUFFER,  8 * vertices_size, 4 * vertices_size, &boneWeights[0]);
		//check_gl_error();
		//
		//glBufferSubData(GL_ARRAY_BUFFER, 12 * vertices_size, 4 * vertices_size, &boneIndicies[0]);
		//check_gl_error();


	}

	//Create Index Buffer
	glGenBuffers(1, &indexBufferID);
	check_gl_error();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	check_gl_error();

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	check_gl_error();

	unsigned int normalOffset = 3 * vertices_size;
	unsigned int textureCoordOffset = 6 * vertices_size;
	unsigned int bitangentOffset = 9 * vertices_size;

	//unsigned int boneWeight = 8 * vertices_size;
	//unsigned int boneIndex = 12 * vertices_size;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	check_gl_error();

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(textureCoordOffset));
	check_gl_error();

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(normalOffset));
	check_gl_error();

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bitangentOffset));
	check_gl_error();


	//if (myAnimation->isAnimated()) {
	//	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(boneWeight));
	//	check_gl_error();
	//
	//	glVertexAttribPointer(4, 4, GL_INT, GL_FALSE, 0, BUFFER_OFFSET(boneIndex));
	//	check_gl_error();
	//
	//	glEnableVertexAttribArray(3);
	//	check_gl_error();
	//
	//	glEnableVertexAttribArray(4);
	//	check_gl_error();
	//
	//}


	glEnableVertexAttribArray(0);
	check_gl_error();

	glEnableVertexAttribArray(1);
	check_gl_error();

	glEnableVertexAttribArray(2);
	check_gl_error();

	glEnableVertexAttribArray(3);
	check_gl_error();
}

bool Mesh::LoadMesh(FbxScene* scene)
{
	keyPress = false;
	lastTime = 0.0f;
	CurTotalTime = 0.0f;

	myAnimations[0] = nullptr;
	myAnimations[1] = nullptr;

	Anim0 = true;
	Anim1 = false;

	cur0 = 0;
	next0 = 0;

	cur1 = 0;
	next1 = 0;

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

		FbxGeometryElementBinormal* vertexBinormal = mesh->GetElementBinormal(0);



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


				if (vertexBinormal != nullptr) {
					glm::vec3 outBinormal;
					outBinormal.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(polyVertIndex).mData[0]);
					outBinormal.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(polyVertIndex).mData[1]);
					outBinormal.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(polyVertIndex).mData[2]);

					biTangents.push_back(outBinormal);
				}

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

bool Mesh::getSameVertexIndex(PackedVertex & packed, std::map<PackedVertex, unsigned int> & VertexToOutIndex, unsigned int & result) {
	std::map<PackedVertex, unsigned int>::iterator it = VertexToOutIndex.find(packed);
	if (it == VertexToOutIndex.end()) {
		return false;
	}
	else {
		result = it->second;
		return true;
	}
}

void Mesh::setActive() {
	glBindVertexArray(vertexArrayObject);
	check_gl_error();

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	check_gl_error();

	if (myAnimation->isAnimated())
	{

		if (GetAsyncKeyState('P'))
		{
			m_entityManager->findEntity("Mage")->getTransform()->RotateY(1);
		}

		if (GetAsyncKeyState(VK_SPACE))
		{
			if (!keyPress)
			{
				if (myAnimations[1] != nullptr && !Anim1)
				{
					Anim1 = true;
				}
				keyPress = true;
			}

		}
		else
		{
			keyPress = false;
		}

		if (Anim1)
		{
			singleFrameTime1 = static_cast<float>(1.0f / myAnimations[1]->getAniLength());
			float deltaTime1 = static_cast<float>(glfwGetTime()) - lastTime1;
			lastTime1 = static_cast<float>(glfwGetTime());
			CurTotalTime1 += deltaTime1;

			if (CurTotalTime1 > singleFrameTime1)
			{
				cur1++;
				CurTotalTime1 = 0.0f;
			}

			

			if (cur1 > myAnimations[1]->getAniLength() - 1)
			{
				Anim1 = false;
				cur1 = 0;
			}
			else if (cur1 == myAnimations[1]->getAniLength() - 1)
			{
				Anim0 = true;
			}

			next1 = cur1 + 1;

			if (next1 > myAnimations[1]->getAniLength() - 1)
			{
				next1 = 0;
			}
		}

		if (Anim0)
		{
			singleFrameTime = static_cast<float>(1.0f / myAnimations[0]->getAniLength());
			float deltaTime = static_cast<float>(glfwGetTime()) - lastTime;
			lastTime = static_cast<float>(glfwGetTime());
			CurTotalTime += deltaTime;

			if (CurTotalTime > singleFrameTime)
			{
				cur0++;
				CurTotalTime = 0.0f;
				if (Anim1)
				{
					Anim0 = false;
					cur0 = 0;
					next0 = 0;
				}
			}

			if (cur0 > myAnimations[0]->getAniLength() - 1)
			{
				cur0 = 0;
			}

			next0 = cur0 + 1;

			if (next0 > myAnimations[0]->getAniLength() - 1)
			{
				next0 = 0;
			}

		}
		


		boneOffsets.clear();

		Skeleton mySkeles[2];
		mySkeles[0] = myAnimations[0]->getSkele();


		if (myAnimations[1])
		{
			mySkeles[1] = myAnimations[1]->getSkele();
		}


		for (unsigned int k = 0; k < mySkeles[0].mJoints.size(); k++)
		{

			FbxAMatrix BindposeInverse = mySkeles[0].mJoints[k].mGlobalBindposeInverse;

			glm::mat4 curBoneOffset;
			curBoneOffset[0] = glm::vec4(BindposeInverse.mData[0][0], BindposeInverse.mData[0][1], BindposeInverse.mData[0][2], BindposeInverse.mData[0][3]);
			curBoneOffset[1] = glm::vec4(BindposeInverse.mData[1][0], BindposeInverse.mData[1][1], BindposeInverse.mData[1][2], BindposeInverse.mData[1][3]);
			curBoneOffset[2] = glm::vec4(BindposeInverse.mData[2][0], BindposeInverse.mData[2][1], BindposeInverse.mData[2][2], BindposeInverse.mData[2][3]);
			curBoneOffset[3] = glm::vec4(BindposeInverse.mData[3][0], BindposeInverse.mData[3][1], BindposeInverse.mData[3][2], BindposeInverse.mData[3][3]);

			glm::mat4 keyFrameLerped;

			if (Anim0)
			{
				FbxAMatrix keyMat = mySkeles[0].mJoints[k].mAnimation[cur0]->mGlobalTransform;

				glm::mat4 keyframe1;
				keyframe1[0] = glm::vec4(keyMat.mData[0][0], keyMat.mData[0][1], keyMat.mData[0][2], keyMat.mData[0][3]);
				keyframe1[1] = glm::vec4(keyMat.mData[1][0], keyMat.mData[1][1], keyMat.mData[1][2], keyMat.mData[1][3]);
				keyframe1[2] = glm::vec4(keyMat.mData[2][0], keyMat.mData[2][1], keyMat.mData[2][2], keyMat.mData[2][3]);
				keyframe1[3] = glm::vec4(keyMat.mData[3][0], keyMat.mData[3][1], keyMat.mData[3][2], keyMat.mData[3][3]);

				FbxAMatrix keyMat2 = mySkeles[0].mJoints[k].mAnimation[next0]->mGlobalTransform;

				glm::mat4 keyframe2;
				keyframe2[0] = glm::vec4(keyMat2.mData[0][0], keyMat2.mData[0][1], keyMat2.mData[0][2], keyMat2.mData[0][3]);
				keyframe2[1] = glm::vec4(keyMat2.mData[1][0], keyMat2.mData[1][1], keyMat2.mData[1][2], keyMat2.mData[1][3]);
				keyframe2[2] = glm::vec4(keyMat2.mData[2][0], keyMat2.mData[2][1], keyMat2.mData[2][2], keyMat2.mData[2][3]);
				keyframe2[3] = glm::vec4(keyMat2.mData[3][0], keyMat2.mData[3][1], keyMat2.mData[3][2], keyMat2.mData[3][3]);

				float t = CurTotalTime / singleFrameTime;


				keyFrameLerped = combineMat4(keyframe1, keyframe2, t);
			}

			if (Anim1)
			{
				FbxAMatrix keyMat = mySkeles[1].mJoints[k].mAnimation[cur1]->mGlobalTransform;

				glm::mat4 keyframe1;
				keyframe1[0] = glm::vec4(keyMat.mData[0][0], keyMat.mData[0][1], keyMat.mData[0][2], keyMat.mData[0][3]);
				keyframe1[1] = glm::vec4(keyMat.mData[1][0], keyMat.mData[1][1], keyMat.mData[1][2], keyMat.mData[1][3]);
				keyframe1[2] = glm::vec4(keyMat.mData[2][0], keyMat.mData[2][1], keyMat.mData[2][2], keyMat.mData[2][3]);
				keyframe1[3] = glm::vec4(keyMat.mData[3][0], keyMat.mData[3][1], keyMat.mData[3][2], keyMat.mData[3][3]);

				FbxAMatrix keyMat2 = mySkeles[1].mJoints[k].mAnimation[next1]->mGlobalTransform;

				glm::mat4 keyframe2;
				keyframe2[0] = glm::vec4(keyMat2.mData[0][0], keyMat2.mData[0][1], keyMat2.mData[0][2], keyMat2.mData[0][3]);
				keyframe2[1] = glm::vec4(keyMat2.mData[1][0], keyMat2.mData[1][1], keyMat2.mData[1][2], keyMat2.mData[1][3]);
				keyframe2[2] = glm::vec4(keyMat2.mData[2][0], keyMat2.mData[2][1], keyMat2.mData[2][2], keyMat2.mData[2][3]);
				keyframe2[3] = glm::vec4(keyMat2.mData[3][0], keyMat2.mData[3][1], keyMat2.mData[3][2], keyMat2.mData[3][3]);

				float t = CurTotalTime1 / singleFrameTime1;


				if (!Anim0)
				{
					keyFrameLerped = combineMat4(keyframe1, keyframe2, t);
				}
				else
				{
					glm::mat4 SecondAnim = combineMat4(keyframe1, keyframe2, t);
					keyFrameLerped = combineMat4(keyFrameLerped, SecondAnim, t);
				}

			}
			

			glm::mat4 finalOffset = keyFrameLerped * curBoneOffset;

			finalOffset = glm::transpose(finalOffset);


			boneOffsets.push_back(finalOffset);


			//keyFrameLerped =(1-t) * keyframe1 + t * keyframe2; //Simple lerp, not good for animation with rotations
		}


		std::vector< glm::vec3 > final_vertices;
		std::vector< glm::vec3 > final_normals;

		int indiciesIndex = 0;
		for (unsigned int i = 0; i < indexed_vertices.size(); i++)
		{

			glm::vec4 curPosition = glm::vec4(indexed_vertices[i], 1) * boneOffsets[boneIndicies[indiciesIndex]] * boneWeights[indiciesIndex];
			curPosition += glm::vec4(indexed_vertices[i], 1) * boneOffsets[boneIndicies[indiciesIndex + 1]] * boneWeights[indiciesIndex + 1];
			curPosition += glm::vec4(indexed_vertices[i], 1) * boneOffsets[boneIndicies[indiciesIndex + 2]] * boneWeights[indiciesIndex + 2];
			curPosition += glm::vec4(indexed_vertices[i], 1) * boneOffsets[boneIndicies[indiciesIndex + 3]] * boneWeights[indiciesIndex + 3];

			final_vertices.push_back(glm::vec3(curPosition));

			glm::vec4 curNorm = (glm::vec4(indexed_normals[i], 0) * boneOffsets[boneIndicies[indiciesIndex]]) * boneWeights[indiciesIndex];
			curNorm += (glm::vec4(indexed_normals[i], 0) * boneOffsets[boneIndicies[indiciesIndex + 1]]) * boneWeights[indiciesIndex + 1];
			curNorm += (glm::vec4(indexed_normals[i], 0) * boneOffsets[boneIndicies[indiciesIndex + 2]]) * boneWeights[indiciesIndex + 2];
			curNorm += (glm::vec4(indexed_normals[i], 0) * boneOffsets[boneIndicies[indiciesIndex + 3]]) * boneWeights[indiciesIndex + 3];

			final_normals.push_back(glm::vec3(curNorm));

			indiciesIndex += 4;
		}

		unsigned int vertices_size = indexed_vertices.size() * sizeof(float);
		unsigned int normalOffset = 3 * vertices_size;

		glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * vertices_size, &final_vertices[0]);
		check_gl_error();

		glBufferSubData(GL_ARRAY_BUFFER, 3 * vertices_size, 3 * vertices_size, &final_normals[0]);
		check_gl_error();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		check_gl_error();


		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(normalOffset));
		check_gl_error();

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

glm::mat4 Mesh::combineMat4(glm::mat4 matrix1, glm::mat4 matrix2, float t)
{
	glm::quat key1Rot = glm::quat_cast(matrix1);
	glm::quat key2Rot = glm::quat_cast(matrix2);

	glm::vec3 key1Pos = glm::vec3(matrix1[3][0], matrix1[3][1], matrix1[3][2]);
	glm::vec3 key2Pos = glm::vec3(matrix2[3][0], matrix2[3][1], matrix2[3][2]);

	glm::vec3 key1Scale;
	key1Scale[0] = glm::length(glm::vec3(matrix1[0][0], matrix1[1][0], matrix1[2][0]));
	key1Scale[1] = glm::length(glm::vec3(matrix1[0][1], matrix1[1][1], matrix1[2][1]));
	key1Scale[2] = glm::length(glm::vec3(matrix1[0][2], matrix1[1][2], matrix1[2][2]));

	glm::vec3 key2Scale;
	key2Scale[0] = glm::length(glm::vec3(matrix2[0][0], matrix2[1][0], matrix2[2][0]));
	key2Scale[1] = glm::length(glm::vec3(matrix2[0][1], matrix2[1][1], matrix2[2][1]));
	key2Scale[2] = glm::length(glm::vec3(matrix2[0][2], matrix2[1][2], matrix2[2][2]));


	glm::quat slerpRot = glm::slerp(key1Rot, key2Rot, t);
	glm::vec3 lerpPos = glm::lerp(key1Pos, key2Pos, t);
	glm::vec3 lerpScale = glm::lerp(key1Scale, key2Scale, t);

	glm::mat4 rotMatrix = glm::mat4_cast(slerpRot);
	rotMatrix[3] = glm::vec4(lerpPos, 1.0f);

	return rotMatrix;
}