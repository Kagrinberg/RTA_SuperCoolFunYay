#include "RenderingManager.h"
#include "ResourceManager.h"
#include "Transform.h"

RenderingManager::RenderingManager(){

	m_numRenderables = 0;

}

void RenderingManager::addRenderable(Renderable * renderable){

	m_renderables[m_numRenderables] = renderable;
	m_numRenderables++;

};

void RenderingManager::RenderAll(){

	for(unsigned int i = 0; i < m_numRenderables; i++){
		Transform * transform = m_renderables[i]->getParent()->getTransform();


		unsigned int meshID = m_renderables[i]->getMeshID();
		unsigned int materialID = m_renderables[i]->getMaterialID();
		Material * material = ResourceManager::getInstance()->getMaterial(materialID);
		unsigned int diffuseID = material->getDiffuseID();
		unsigned int specularID = material->getSpecularID();
		unsigned int texture0ID = ResourceManager::getInstance()->getTexture(diffuseID)->getTexID();
		unsigned int texture1ID = ResourceManager::getInstance()->getTexture(specularID)->getTexID();
		unsigned int matDiffuseLoc = glGetUniformLocation(3, "material.diffuse");
		unsigned int matSpecularLoc = glGetUniformLocation(3, "material.specular");
		unsigned int matShininessLoc = glGetUniformLocation(3, "material.shininess");
		glUniform1i(matDiffuseLoc, 0);
		glUniform1i(matSpecularLoc, 1);
		glUniform1f(matShininessLoc, material->getShininess());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0ID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1ID);
		Mesh * mesh = ResourceManager::getInstance()->getMesh(meshID);
		mesh->setActive();
		glDrawElements(GL_TRIANGLES, mesh->getIndices().size(), GL_UNSIGNED_INT, NULL);
	}

};


