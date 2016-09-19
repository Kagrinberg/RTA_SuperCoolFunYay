#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 bitangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
	mat3 TBN;
	vec3 Normal;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main () {

	gl_Position = projection *view * model * vec4(position,1);
    vs_out.FragPos = vec3(model * vec4(position.xyz, 1.0f));
    vs_out.TexCoords = vec2(texCoords.x, 1.0 - texCoords.y);

	vs_out.Normal = mat3(transpose(inverse(model))) * normal;  

	vec3 tangent = cross(normal, bitangent);

	mat3 normalMatrix = mat3(model);
    vec3 T = normalize(normalMatrix * tangent);
    vec3 B = normalize(normalMatrix * bitangent);
    vec3 N = normalize(normalMatrix * normal);    
    
    vs_out.TBN = mat3(T, B, N);  


}