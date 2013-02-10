//
//  GameHUD.cpp
//  
//
//  Created by Bryan Bell on 2/9/13.
//
//

#include "GameHUD.h"

extern ModelManager *g_model_manager;

GameHUD::GameHUD(float width, float height)
{
   bound boundingInfo;
   g_model_manager->getObject("plane", &meshStorage, &boundingInfo);
   
   hudWidth = width;
   hudHeight = height;
   
#ifdef DEBUG_VBO
   //printf("VBO Arrived at its Destination: %d\n", (int)indexBufferLength[0]);
#endif
}

void GameHUD::drawSpeed(PhongShader *meshShader, RenderingHelper modelViewMatrix, float speed)
{
   GLuint h_aPos, h_aNorm;
   h_aPos = meshShader->getPosLocation();
   h_aNorm = meshShader->getNormLocation();
   meshShader->use();
   
   modelViewMatrix.pushMatrix();
      modelViewMatrix.loadIdentity();
   
      modelViewMatrix.translate(vec3(hudWidth - 50.0, hudHeight - 50.0, 1.0));
      modelViewMatrix.scale(100.0);
      //modelViewMatrix.rotate(rot.x, vec3(1.0, 0.0, 0.0));
      //modelViewMatrix.rotate(rot.y, vec3(0.0, 1.0, 0.0));
      //modelViewMatrix.rotate(rot.z, vec3(0.0, 0.0, 1.0));
      meshShader->setModelMatrix(modelViewMatrix.getMatrix());
      meshShader->setiModelMatrix(glm::transpose(glm::inverse(modelViewMatrix.getMatrix())));
   
      safe_glEnableVertexAttribArray(h_aPos);
      glBindBuffer(GL_ARRAY_BUFFER, meshStorage.vertexBuffer);
      safe_glVertexAttribPointer(h_aPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
      safe_glEnableVertexAttribArray(h_aNorm);
      glBindBuffer(GL_ARRAY_BUFFER, meshStorage.normalBuffer);
      safe_glVertexAttribPointer(h_aNorm, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
      for (int i = 0; i < meshStorage.numMeshes; i++) {
         //printf("We are drawing, right? %d\n", indexBufferLength[i]);
      
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshStorage.indexBuffer[i]);
      
         //printf("Number of Faces: %d\n", meshStorage.indexBuffer[i]);
         glDrawElements(GL_TRIANGLES, meshStorage.indexBufferLength[i], GL_UNSIGNED_SHORT, 0);
      }
   
      safe_glDisableVertexAttribArray(h_aPos);
      safe_glDisableVertexAttribArray(h_aNorm);
   
   modelViewMatrix.popMatrix();
   
   PhongMaterial mat;
   mat.dColor = vec3(1.0, 0.0, 0.0);
   mat.sColor = vec3(1.0, 1.0, 1.0);
   mat.aColor = vec3(1.0, 0.0, 0.0);
   mat.shine = 0.0;
   
   meshShader->setMaterial(mat);
   
   modelViewMatrix.pushMatrix();
      modelViewMatrix.loadIdentity();
   
      modelViewMatrix.translate(vec3(hudWidth - 50.0, hudHeight - 50.0, 1.0));
      modelViewMatrix.scale(75.0);
      //modelViewMatrix.rotate(rot.x, vec3(1.0, 0.0, 0.0));
      //modelViewMatrix.rotate(rot.y, vec3(0.0, 1.0, 0.0));
      modelViewMatrix.rotate(180 * (speed/50.0), vec3(0.0, 0.0, 1.0));
      meshShader->setModelMatrix(modelViewMatrix.getMatrix());
      meshShader->setiModelMatrix(glm::transpose(glm::inverse(modelViewMatrix.getMatrix())));
   
      safe_glEnableVertexAttribArray(h_aPos);
      glBindBuffer(GL_ARRAY_BUFFER, meshStorage.vertexBuffer);
      safe_glVertexAttribPointer(h_aPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
      safe_glEnableVertexAttribArray(h_aNorm);
      glBindBuffer(GL_ARRAY_BUFFER, meshStorage.normalBuffer);
      safe_glVertexAttribPointer(h_aNorm, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
      for (int i = 0; i < meshStorage.numMeshes; i++) {
         //printf("We are drawing, right? %d\n", indexBufferLength[i]);
      
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshStorage.indexBuffer[i]);
      
         //printf("Number of Faces: %d\n", meshStorage.indexBuffer[i]);
         glDrawElements(GL_TRIANGLES, meshStorage.indexBufferLength[i], GL_UNSIGNED_SHORT, 0);
      }
   
      safe_glDisableVertexAttribArray(h_aPos);
      safe_glDisableVertexAttribArray(h_aNorm);
   
   modelViewMatrix.popMatrix();
}