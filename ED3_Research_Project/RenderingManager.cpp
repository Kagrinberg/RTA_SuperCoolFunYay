#include "RenderingManager.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "glm/gtc/type_ptr.hpp"
#include <algorithm>
#include <vector>
#include <iostream>   
#include "GLError.h"
#include "Registry.h"


RenderingManager::RenderingManager(){

	m_numRenderables = 0;
	m_currentMaterialID = 0;

}

void RenderingManager::addRenderable(Renderable * renderable){

	m_renderables[m_numRenderables] = renderable;
	m_numRenderables++;

};

void RenderingManager::RenderAll(){

	for(unsigned int i = 0; i < m_numRenderables; i++){
		Transform * transform = m_renderables[i]->getParent()->getTransform();
		glm::mat4 * matrix = transform->getMatrix();

		unsigned int program = 3;

		//if (mesh->isAnimated())
		//{
		//	program = 6;
		//}
		//else
		//{
		//	program = 3;
		//}

		//glUseProgram(program);
		//check_gl_error();

		glUniformMatrix4fv(5, 1, GL_FALSE, glm::value_ptr(*matrix));
		check_gl_error();

		unsigned int materialID = m_renderables[i]->getMaterialID();
		if (m_currentMaterialID != materialID) {
			m_currentMaterialID = materialID;
			Material * material = m_resourceManager->getMaterial(materialID);
			unsigned int diffuseID = material->getDiffuseID();
			unsigned int specularID = material->getSpecularID();
			unsigned int texture0ID = m_resourceManager->getTexture(diffuseID)->getTexID();
			unsigned int texture1ID = m_resourceManager->getTexture(specularID)->getTexID();

			unsigned int matDiffuseLoc = glGetUniformLocation(3, "material.diffuse");
			check_gl_error();

			unsigned int matSpecularLoc = glGetUniformLocation(3, "material.specular");
			check_gl_error();

			unsigned int matShininessLoc = glGetUniformLocation(3, "material.shininess");
			check_gl_error();

			glUniform1i(matDiffuseLoc, 0);
			check_gl_error();

			glUniform1i(matSpecularLoc, 1);
			check_gl_error();

			glUniform1f(matShininessLoc, material->getShininess());
			check_gl_error();

			glActiveTexture(GL_TEXTURE0);
			check_gl_error();

			glBindTexture(GL_TEXTURE_2D, texture0ID);
			check_gl_error();

			glActiveTexture(GL_TEXTURE1);
			check_gl_error();

			glBindTexture(GL_TEXTURE_2D, texture1ID);
			check_gl_error();

		}
		unsigned int meshID = m_renderables[i]->getMeshID();

		Mesh * mesh = m_resourceManager->getMesh(meshID);

		mesh->setActive();

		glDrawElements(GL_TRIANGLES, mesh->getIndices().size(), GL_UNSIGNED_INT, NULL);
		check_gl_error();


	}

};


void RenderingManager::sortByMaterial() {

	Renderable * swap;
	for (unsigned int i = 0; i < (m_numRenderables - 1); i++)
	{
		for (unsigned int j = 0; j < m_numRenderables - i - 1; j++)
		{
			if (m_renderables[j]->getMaterialID() > m_renderables[j + 1]->getMaterialID()) /* For decreasing order use < */
			{
				swap = m_renderables[j];
				m_renderables[j] = m_renderables[j + 1];
				m_renderables[j + 1] = swap;
			}
		}
	}
	unsigned int a = 0;
};

bool RenderingManager::sortFunction(Renderable * i, Renderable * j) {

	return (i->getMaterialID() < j->getMaterialID());

}