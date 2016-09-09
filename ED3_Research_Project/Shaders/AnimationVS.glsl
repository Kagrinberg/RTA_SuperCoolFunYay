#version 450

#define MAX_BONES 128 

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec4 normal;
layout (location = 3) in float4 boneWeight;
layout (location = 4) in int4 boneIndex;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 BoneOffset[MAX_BONES];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main () {

	gl_Position = projection *view * model * position;

    FragPos = vec3(model * vec4(position.xyz, 1.0f));
    Normal = mat3(transpose(inverse(model))) * normal.xyz;  
    TexCoords = texCoords;


}