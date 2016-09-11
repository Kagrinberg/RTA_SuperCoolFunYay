#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec4 normal;

//layout (std140) uniform Matrices
//{
//    mat4 projection;
//    mat4 view;
//};

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main () {

	gl_Position = projection *view * model * position;

    FragPos = vec3(model * vec4(position.xyz, 1.0f));
    Normal = mat3(transpose(inverse(model))) * normal.xyz;  


    TexCoords = texCoords;

}