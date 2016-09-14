#version 450
layout (location = 0) in vec3 position;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position =   projection * view * vec4(position, 1.0);  
	gl_Position.z = gl_Position.w -0.00001;
    TexCoords = position;
}  