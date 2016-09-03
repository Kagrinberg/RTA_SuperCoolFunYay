#version 430

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

in vec3 fN;
in vec3 fL;
in vec3 fE;
in vec2 texCoord;			
	
uniform Material material;

out vec4 fColor;

void main () {
	vec3 N = normalize(fN);
	vec3 L = normalize(fL);

	vec3 E = normalize(-fE);
	vec3 H = normalize(L + E);
	
	float diffuse_intensity = max(dot(N,L), 0.0);
	vec4 texDiffuse = texture(material.diffuse, texCoord);
	vec4 diffuse_final = vec4((diffuse_intensity * texDiffuse.xyz), texDiffuse.w);

	float spec_intensity = pow(max(dot(N,H), 0.0), material.shininess);
	vec4 texSpecular = texture(material.specular, texCoord);
	vec4 spec_final = vec4((spec_intensity * texSpecular.xyz), texSpecular.w);
	
	fColor = spec_final + diffuse_final;
}