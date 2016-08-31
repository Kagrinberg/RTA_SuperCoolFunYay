#version 450

layout(location = 0) in vec4 s_vPosition;
layout(location = 1) in vec2 s_vTexCoord;
layout(location = 2) in vec4 s_vNormal;

uniform mat4 mM;
uniform mat4 mV;
uniform mat4 mP;
uniform vec4 vLight;

out vec3 fN;
out vec3 fL;
out vec3 fE;	

out vec2 texCoord;

void main () {


    fN = mat3(transpose(inverse(mM))) * s_vNormal.xyz;
    fL = vLight.xyz;
    fE = (mV*mM*s_vPosition).xyz;
    texCoord = s_vTexCoord;

	gl_Position = mP*mV*mM*s_vPosition;

}