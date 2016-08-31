#include "Material.h"
//#include <GL\glew.h>

//void Material::LoadEntry(){

	    //glUniform4f(glGetUniformLocation(4, "material.ambient"), this->ambient.x, this->ambient.y, this->ambient.z, this->ambient.w);
     //   glUniform4f(glGetUniformLocation(4, "material.diffuse"), this->diffuse.x, this->diffuse.y, this->diffuse.z, this->diffuse.w);
     //   glUniform4f(glGetUniformLocation(4, "material.specular"), this->specular.x, this->specular.y, this->specular.z, this->specular.w); // Specular doesn't have full effect on this object's material
	
	//glUniform1f(glGetUniformLocation(4, "material.shininess"), this->shininess);

//};

	//vec3 N = normalize(fN);
	//vec3 L = normalize(fL);

	//vec3 E = normalize(-fE);
	//vec3 H = normalize(L + E);

	//// Ambient
 //   //vec3 ambient = light.ambient * material.ambient;
	//
	////Diffuse
	//float diffuse_intensity = max(dot(N,L), 0.0);
	//vec4 textureColor = texture2D(texture, texCoord);
	//vec4 diffuse_final = (diffuse_intensity * textureColor * material.diffuse);
	////light.diffuse * 

	////Speuclar
	//float spec_intensity = pow(max(dot(N,H), 0.0), material.shininess);
	//vec4 spec_final = (spec_intensity * material.specular);
	//spec_final.w = textureColor.w;

	////light.specular * 
	//
	//fColor = diffuse_final + spec_final;