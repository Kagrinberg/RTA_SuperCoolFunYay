#version 450

#define MAX_BONES 4

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec4 boneWeight;
layout (location = 4) in ivec4 boneIndex;

//layout (std140) uniform Matrices
//{
 //   mat4 projection;
 //   mat4 view;
//};

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 BoneOffset[4];

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main () {

int i = 0;
int j = 1;
int k = 2;
int l = 3;
	gl_Position = vec4(position, 1) * BoneOffset[i] * boneWeight[0];
	gl_Position += vec4(position, 1) * BoneOffset[j] * boneWeight[1];
	gl_Position += vec4(position, 1) * BoneOffset[k] * boneWeight[2];
	gl_Position += vec4(position, 1) * BoneOffset[l] * boneWeight[3];

	//gl_Position =vec4(position,1);
	//gl_Position = vec4(gl_Position.xyz,1);
	gl_Position = projection *view * model * gl_Position;

    FragPos = vec3(model * vec4(position.xyz, 1.0f));
	vec4 tempNorm;

	tempNorm = (vec4(normal, 0) * BoneOffset[0]) * boneWeight[0];
	tempNorm += (vec4(normal, 0) * BoneOffset[1]) * boneWeight[1];
	tempNorm += (vec4(normal, 0) * BoneOffset[2]) * boneWeight[2];
	tempNorm += (vec4(normal, 0) * BoneOffset[3]) * boneWeight[3];

    tempNorm = mat4(transpose(inverse(model))) * tempNorm;  

	Normal = tempNorm.xyz;


    TexCoords = vec2(texCoords.x, 1.0f - texCoords.y);


}