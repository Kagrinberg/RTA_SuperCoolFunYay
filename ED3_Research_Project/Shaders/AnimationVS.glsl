#version 450

#define MAX_BONES 128 

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
uniform mat4 BoneOffset[MAX_BONES];

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main () {


	gl_Position = (vec4(position, 1) * BoneOffset[boneIndex[0]]) * boneWeight[boneIndex[0]];
	gl_Position += (vec4(position, 1) * BoneOffset[boneIndex[1]]) * boneWeight[boneIndex[1]];
	gl_Position += (vec4(position, 1) * BoneOffset[boneIndex[2]]) * boneWeight[boneIndex[2]];
	gl_Position += (vec4(position, 1) * BoneOffset[boneIndex[3]]) * boneWeight[boneIndex[3]];

	gl_Position = projection *view * model * gl_Position;

    FragPos = vec3(model * vec4(position.xyz, 1.0f));

	vec4 tempNorm;
	tempNorm = (vec4(normal, 0) * BoneOffset[boneIndex[0]]) * boneWeight[boneIndex[0]];
	tempNorm += (vec4(normal, 0) * BoneOffset[boneIndex[1]]) * boneWeight[boneIndex[1]];
	tempNorm += (vec4(normal, 0) * BoneOffset[boneIndex[2]]) * boneWeight[boneIndex[2]];
	tempNorm += (vec4(normal, 0) * BoneOffset[boneIndex[3]]) * boneWeight[boneIndex[3]];
    tempNorm = mat4(transpose(inverse(model))) * tempNorm;  

	Normal = tempNorm.xyz;


    TexCoords = texCoords;


}