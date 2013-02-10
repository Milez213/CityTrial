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

extern ModelManager *g_model_manager;

GameDrawableObject::GameDrawableObject(const char *objFile) : toRemove(false)
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
   
   /*glGenVertexArrays(1, &vertexArray);
   glBindVertexArray(vertexArray);
   
   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
   glVertexAttribPointer(POS_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(POS_LOCATION);
   
   glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
   glVertexAttribPointer(NORM_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(NORM_LOCATION);
   
   glBindVertexArray(0);*/
}


void GameDrawableObject::draw(PhongShader *meshShader, RenderingHelper modelViewMatrix)
{
   //cout << "draw\n";
   modelViewMatrix.pushMatrix();
   meshShader->use();
   
   modelViewMatrix.translate(getPosition());
   modelViewMatrix.scale(scl.x, scl.y, scl.z);
   modelViewMatrix.rotate(rot.x, vec3(1.0, 0.0, 0.0));
   modelViewMatrix.rotate(rot.y, vec3(0.0, 1.0, 0.0));
   modelViewMatrix.rotate(rot.z, vec3(0.0, 0.0, 1.0));
   meshShader->setModelMatrix(modelViewMatrix.getMatrix());
   meshShader->setiModelMatrix(glm::transpose(glm::inverse(modelViewMatrix.getMatrix())));
   
   //glBindVertexArray(vertexArray);
   
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
}

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
   
   boundingInfo.bottomLeft = p-s;
   boundingInfo.dimension = 2.0f * s;
   boundingInfo.radius = std::max(std::max(s.x, s.y), s.z);//glm::length(s);//
   boundingInfo.center = p;
}

bound GameDrawableObject::getBoundingInfo()
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
