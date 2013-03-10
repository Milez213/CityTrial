/*
 * GameDrawableObject.h
 * Author: Chris Pauley & Bryan Bell
 *
 * Date: 1/28/2013
 * This class defines a base drawable game
 *  object. Its only unique attributes from
 *  "GameObject" are its buffer objects used
 *  to render the object.
 */

#include "GameDrawableObject.h"

#include "include_glm.h"


extern ModelManager *g_model_manager;

GameDrawableObject::GameDrawableObject(const char *objFile) : GameObject(), toRemove(false)
{
   g_model_manager->getObject(objFile, &meshStorage, &boundingInfo);

#ifdef DEBUG_VBO
   printf("VBO Arrived at its Destination: %d\n", (int)indexBufferLength[0]);
#endif
   //dir = vec3(0.0, 0.0, 1.0);
   //spd = 0.0;
   
   //pos = vec3(0.0, 0.0, 5.0);
   rot = vec3(0.0, 0.0, 0.0);
   setScale(vec3(1.0, 1.0, 1.0));
   
   toRemove = false;
   
   materialIndex = -1;
}


void GameDrawableObject::transform(RenderingHelper &modelViewMatrix)
{
   modelViewMatrix.translate(getPosition());
   modelViewMatrix.scale(scl.x, scl.y, scl.z);
   modelViewMatrix.rotate(rot.x, vec3(1.0, 0.0, 0.0));
   modelViewMatrix.rotate(rot.y, vec3(0.0, 1.0, 0.0));
   modelViewMatrix.rotate(rot.z, vec3(0.0, 0.0, 1.0));
}


void GameDrawableObject::draw(PhongShader *meshShader, RenderingHelper modelViewMatrix)
{
   modelViewMatrix.pushMatrix();
   meshShader->use();
   
   transform(modelViewMatrix);
   meshShader->setModelMatrix(modelViewMatrix.getMatrix());
   
   
   GLuint h_aPos = meshShader->getPosLocation();
   GLuint h_aNorm = meshShader->getNormLocation();
   // TODO - add texture buffer
   
   safe_glEnableVertexAttribArray(h_aPos);
   glBindBuffer(GL_ARRAY_BUFFER, meshStorage.vertexBuffer);
   safe_glVertexAttribPointer(h_aPos, 3, GL_FLOAT, GL_FALSE, 0, 0);

   safe_glEnableVertexAttribArray(h_aNorm);
   glBindBuffer(GL_ARRAY_BUFFER, meshStorage.normalBuffer);
   safe_glVertexAttribPointer(h_aNorm, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   for (int i = meshStorage.numMeshes - 1; i >= 0; i--) {
      meshShader->setMaterial(&meshStorage.material[i]);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshStorage.indexBuffer[i]);
      glDrawElements(GL_TRIANGLES, meshStorage.indexBufferLength[i], GL_UNSIGNED_SHORT, 0);
   }
   
   safe_glDisableVertexAttribArray(h_aPos);
   safe_glDisableVertexAttribArray(h_aNorm);
   
   modelViewMatrix.popMatrix();
   /*modelViewMatrix.pushMatrix();
   
   modelViewMatrix.translate(getPosition());
   modelViewMatrix.scale(scl.x, scl.y, scl.z);
   meshShader->setModelMatrix(modelViewMatrix.getMatrix());
   
   safe_glEnableVertexAttribArray(h_aNorm);
   glBindBuffer(GL_ARRAY_BUFFER, meshStorage.normalBuffer);
   safe_glVertexAttribPointer(h_aNorm, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   safe_glEnableVertexAttribArray(h_aPos);
   glBindBuffer(GL_ARRAY_BUFFER, meshStorage.shadowVBO);
   safe_glVertexAttribPointer(h_aPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   for (int i = meshStorage.numMeshes - 1; i >= 0; i--) {
      meshShader->setMaterial(&meshStorage.material[i]);
      if (meshStorage.shadowVBO != NULL) {
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshStorage.shadowIBO[i]);
         glDrawElements(GL_TRIANGLES, meshStorage.shadowBufferLength[i], GL_UNSIGNED_SHORT, 0);
      }
   }
   
   safe_glDisableVertexAttribArray(h_aPos);
   safe_glDisableVertexAttribArray(h_aNorm);
   
   modelViewMatrix.popMatrix();*/
}

/*void GameDrawableObject::drawSpecial(PhongShader *meshShader, RenderingHelper modelViewMatrix, float pitchAngle, float rollAngle)
{
   modelViewMatrix.pushMatrix();
   meshShader->use();
   
   modelViewMatrix.translate(getPosition());
   modelViewMatrix.scale(scl.x, scl.y, scl.z);
   modelViewMatrix.rotate(rot.x, vec3(1.0, 0.0, 0.0));
   modelViewMatrix.rotate(rot.y, vec3(0.0, 1.0, 0.0));
   modelViewMatrix.rotate(rot.z, vec3(0.0, 0.0, 1.0));
   modelViewMatrix.rotate(pitchAngle,vec3(0.0,0.0,1.0));
   modelViewMatrix.rotate(-rollAngle,vec3(1.0,0.0,0.0));
   meshShader->setModelMatrix(modelViewMatrix.getMatrix());
   
   
   GLuint h_aPos = meshShader->getPosLocation();
   GLuint h_aNorm = meshShader->getNormLocation();
   // TODO - add texture buffer
   
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
}*/


// draw using GameObject's transform info, not a matrix stack.
// draws in world coordinates, so this doesn't use hiearchical modeling
/*void GameDrawableObject::draw(PhongShader *meshShader) {
   meshShader->use();

   mat4 modelMat = glm::rotate(mat4(1.0f), rot.x, vec3(1.0, 0.0, 0.0));
   modelMat = glm::rotate(modelMat, rot.y, vec3(0.0, 1.0, 0.0));
   modelMat = glm::rotate(modelMat, rot.z, vec3(0.0, 0.0, 1.0));
   
   modelMat = glm::scale(modelMat, scl);
   modelMat = glm::translate(glm::mat4(1.0f), pos) * modelMat;
   
   meshShader->setModelMatrix(modelMat);
   
   
   GLuint h_aPos = meshShader->getPosLocation();
   GLuint h_aNorm = meshShader->getNormLocation();
   // TODO - add texture buffer
   
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
   
}*/

void GameDrawableObject::onCollide(GameDrawableObject *other)
{
   // cout << "outch!" << other->getPosition().x << "\n";
}


void GameDrawableObject::setScale(vec3 s)
{
   GameObject::setScale(s);
   updateBoundingInfo();
}
void GameDrawableObject::setPosition(vec3 p)
{
   GameObject::setPosition(p);
   updateBoundingInfo();
}

void GameDrawableObject::updateBoundingInfo()
{
   
   vec3 s = getScale();
   vec3 p = getPosition();
   
   // FIXME - these assume object's height is 2
   // boundingInfo.bottomLeft = (boundingInfo.bottomLeft - p) * s + p;

   //boundingInfo.bottomLeft = p-s;
   // test scale bottomLeft. from origin
   boundingInfo.dimension = s;
   boundingInfo.radius = std::max(std::max(s.x, s.y), s.z);//glm::length(s);//
   boundingInfo.center = p;

}

// TODO - change bounding box so it's at origin. Then translate when returning it.
const bound GameDrawableObject::getBoundingInfo()
{
   /*bound chgBound;
   chgBound.bottomLeft = boundingInfo.bottomLeft + this->getPosition();
   chgBound.dimension = boundingInfo.dimension * this->getScale();
   chgBound.center = boundingInfo.center + this->getPosition();
   
   if (this->getScale().x > this->getScale().y) {
      if (this->getScale().x > this->getScale().z) {
         chgBound.radius *= this->getScale().x;
      } else {
         chgBound.radius *= this->getScale().z;
      }
   } else {
      if (this->getScale().y > this->getScale().z) {
         chgBound.radius *= this->getScale().y;
      } else {
         chgBound.radius *= this->getScale().z;
      }
   }
   
   return chgBound;*/
   
   
   return boundingInfo;
}
