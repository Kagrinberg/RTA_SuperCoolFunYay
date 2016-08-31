#include "Mesh.h"

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

bool Mesh::LoadEntry(const char * path){

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
			unsigned int newindex = (unsigned int)indexed_vertices.size() -1;
			indices.push_back(newindex);
			VertexToOutIndex[packed] = newindex;
		}

	}

}

void Mesh::GenerateBuffers(){

	//Create Array Object
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	//Create Vertex Buffer
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

	unsigned int num_vertices = indexed_vertices.size();
	unsigned int vertices_size = num_vertices * sizeof(float);

	glBufferData(GL_ARRAY_BUFFER, 8*vertices_size, NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, 3*vertices_size, &indexed_vertices[0]);

	glBufferSubData(GL_ARRAY_BUFFER, 3*vertices_size, 3*vertices_size, &indexed_normals[0]);

	glBufferSubData(GL_ARRAY_BUFFER, 6*vertices_size, 2*vertices_size, &indexed_uvs[0]);



	//Create Index Buffer
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0] , GL_STATIC_DRAW);


	unsigned int textureCoordOffset = 6*vertices_size;
	unsigned int normalOffset = 3*vertices_size;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(textureCoordOffset));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(normalOffset));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
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
}