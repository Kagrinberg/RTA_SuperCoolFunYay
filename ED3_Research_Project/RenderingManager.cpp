#include "RenderingManager.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "glm/gtc/type_ptr.hpp"
#include <algorithm>
#include <vector>
#include <iostream>   
#include "GLError.h"
#include "Registry.h"


RenderingManager::RenderingManager() {

	m_numRenderables = 0;
	m_currentMaterialID = 0;

}

void RenderingManager::addRenderable(Renderable * renderable) {

	m_renderables[m_numRenderables] = renderable;
	m_numRenderables++;

};

void RenderingManager::RenderAll(Camera & c, LightingManager * lm) {

	for (unsigned int i = 0; i < m_numRenderables; i++) {
		Transform * transform = m_renderables[i]->getParent()->getTransform();
		glm::mat4 * matrix = transform->getMatrix();

		unsigned int program = 3;

		unsigned int meshID = m_renderables[i]->getMeshID();
		Mesh * mesh = m_resourceManager->getMesh(meshID);

		if (mesh->isAnimated()) {
			program = 6;
		}
		else {
			program = 3;
		}

		glUseProgram(program);

		perspectiveMatrixID = glGetUniformLocation(program, "projection");
		check_gl_error();

		viewMatrixID = glGetUniformLocation(program, "view");
		check_gl_error();

		viewPosID = glGetUniformLocation(program, "viewPos");
		check_gl_error();

		modelMatrixID = glGetUniformLocation(program, "model");
		check_gl_error();


		projectionMatrix = glm::perspectiveFov(1.0f / tan(60.0f*3.1415926f / 360.0f), (float)1024, (float)720, 0.001f, 1000.0f);

		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr(c.getMatrix()));
		check_gl_error();
		glUniformMatrix4fv(perspectiveMatrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		check_gl_error();
		glUniform3f(viewPosID, c.getPosition().x, c.getPosition().y, c.getPosition().z);
		check_gl_error();

		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(*matrix));
		check_gl_error();

		unsigned int materialID = m_renderables[i]->getMaterialID();
		if (m_currentMaterialID != materialID) {
			m_currentMaterialID = materialID;
			Material * material = m_resourceManager->getMaterial(materialID);
			unsigned int diffuseID = material->getDiffuseID();
			unsigned int specularID = material->getSpecularID();
			unsigned int texture0ID = m_resourceManager->getTexture(diffuseID)->getTexID();
			unsigned int texture1ID = m_resourceManager->getTexture(specularID)->getTexID();

			unsigned int matDiffuseLoc = glGetUniformLocation(program, "material.diffuse");
			check_gl_error();

			unsigned int matSpecularLoc = glGetUniformLocation(program, "material.specular");
			check_gl_error();

			unsigned int matShininessLoc = glGetUniformLocation(program, "material.shininess");
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

		mesh->setActive();

		glDrawElements(GL_TRIANGLES, mesh->getIndices().size(), GL_UNSIGNED_INT, NULL);
		check_gl_error();
		glUseProgram(0);


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