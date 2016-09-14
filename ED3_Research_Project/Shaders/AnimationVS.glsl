#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main () {




	gl_Position = projection *view * model * vec4(position,1);

    FragPos = vec3(model * vec4(position.xyz, 1.0f));


    Normal = (mat4(transpose(inverse(model))) * vec4(normal,0)).xyz;  


    TexCoords = vec2(texCoords.x, 1.0f - texCoords.y);


}