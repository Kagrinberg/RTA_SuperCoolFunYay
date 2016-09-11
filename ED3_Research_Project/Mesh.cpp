#include "Mesh.h"
#include "GLError.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

Mesh::~Mesh() {

	delete myAnimation;

}

bool Mesh::LoadEntry(const char * path){
	//old obj leader
/*
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	FILE * file;
	fopen_s(&file, path, "r");
	if( file == NULL ){
		printf("Impossible to open the file !\n");
		return false;
	}

	while( true ){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf_s(file, "%s", lineHeader, 128);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z, 128);
			temp_vertices.push_back(vertex);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			glm::vec2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y, 128);
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z, 128);
			temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], 128);
			if (matches != 9){
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			for (unsigned int i = 0; i < 3; i++){
				vertexIndices.push_back(vertexIndex[i]);
				uvIndices.push_back(uvIndex[i]);
				normalIndices.push_back(normalIndex[i]);
			}
		}
	}
	// For each vertex of each triangle
	for( unsigned int i=0; i < vertexIndices.size(); i++ ){
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		vertices.push_back(vertex);
	}
	for( unsigned int i=0; i < uvIndices.size(); i++ ){
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		uvs.push_back(uv);
	}
	for( unsigned int i=0; i < uvIndices.size(); i++ ){
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];
		normals.push_back(normal);
	}
	//Generate New Indices
	GenerateIndices();

	GenerateBuffers();
*/
	return true;
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

			for (unsigned int j = 0; j < temp->jointIndex.size(); j++)
			{
				boneIndicies.push_back(temp->jointIndex[j]);
				boneWeights.push_back(temp->jointWeights[j]);
			}

		}

		Skeleton mySkele = myAnimation->getSkele();

		for (unsigned int k = 0; k < mySkele.mJoints.size(); k++)
		{
			glm::mat4 tempBone;
			tempBone[0] = glm::vec4(mySkele.mJoints[k].mGlobalBindposeInverse.mData[0][0], mySkele.mJoints[k].mGlobalBindposeInverse.mData[0][1], mySkele.mJoints[k].mGlobalBindposeInverse.mData[0][2], mySkele.mJoints[k].mGlobalBindposeInverse.mData[0][3]);
			tempBone[1] = glm::vec4(mySkele.mJoints[k].mGlobalBindposeInverse.mData[1][0], mySkele.mJoints[k].mGlobalBindposeInverse.mData[1][1], mySkele.mJoints[k].mGlobalBindposeInverse.mData[1][2], mySkele.mJoints[k].mGlobalBindposeInverse.mData[1][3]);
			tempBone[2] = glm::vec4(mySkele.mJoints[k].mGlobalBindposeInverse.mData[2][0], mySkele.mJoints[k].mGlobalBindposeInverse.mData[2][1], mySkele.mJoints[k].mGlobalBindposeInverse.mData[2][2], mySkele.mJoints[k].mGlobalBindposeInverse.mData[2][3]);
			tempBone[3] = glm::vec4(mySkele.mJoints[k].mGlobalBindposeInverse.mData[3][0], mySkele.mJoints[k].mGlobalBindposeInverse.mData[3][1], mySkele.mJoints[k].mGlobalBindposeInverse.mData[3][2], mySkele.mJoints[k].mGlobalBindposeInverse.mData[3][3]);

			tempBone = glm::mat4(1.0f);

			boneOffsets.push_back(tempBone);
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
		unsigned int location = glGetUniformLocation(6, "BoneOffset");
		
		glUniformMatrix4fv(location, boneOffsets.size(), GL_FALSE, glm::value_ptr(boneOffsets[0]));
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