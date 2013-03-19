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
#include "Octree.h"
#include "util.h"
#include <cstring>
#include <vector>

PhongMaterial shadowMat = {
   vec3(0.1, 0.1, 0.1), // amb
   vec3(0.0, 0.0, 0.0), // diff
   vec3(-1.0, -1.0, -1.0),       // spec
   20.0 };

extern Octree drawable_objects;

extern ModelManager *g_model_manager;


string appendInt(string &str, int i) {
   int extPos = str.find('.');
   
   return str.substr(0, extPos) + '_' + toString(i) + str.substr(extPos);
}

GameDrawableObject::GameDrawableObject(const char *objFile) : GameObject(), toRemove(false)
{
   //int fileNameLength = strlen(objFile);
   //char *fileName = (char *)malloc(sizeof(char) * (fileNameLength+3));
   string fileName(objFile), tempName;
   //strcpy(fileName, objFile);
   
   meshStorage.resize(1);
   g_model_manager->getObject(fileName.c_str(), &meshStorage[0], &boundingInfo);
   
   for (int i = 1; ifstream((tempName = appendInt(fileName, i)).c_str()).good(); i++) {
      //printf("LoD loaded\n");
      meshStorage.resize(i+1);
      g_model_manager->getObject(tempName.c_str(), &meshStorage[i], &boundingInfo);
   }
   //strcpy(&fileName[fileNameLength], "_1");
   //g_model_manager->getObject(fileName, &meshStorage[1], &boundingInfo);
   //strcpy(&fileName[fileNameLength], "_2");
   //g_model_manager->getObject(fileName, &meshStorage[2], &boundingInfo);
   
   //free(fileName);

#ifdef DEBUG_VBO
   printf("VBO Arrived at its Destination: %d\n", (int)indexBufferLength[0]);
#endif
   //dir = vec3(0.0, 0.0, 1.0);
   //spd = 0.0;
   
   //pos = vec3(0.0, 0.0, 5.0);
   rot = vec3(0.0, 0.0, 0.0);
   setScale(vec3(1.0, 1.0, 1.0));
   
   toRemove = false;
   hasShadow = true;
   
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


void GameDrawableObject::draw(PhongShader *meshShader, RenderingHelper modelViewMatrix, float levelOfDetail)
{
   int LoD = lodIndex(levelOfDetail);
   //printf("levelOfDetail: %f LoD: %d\n",levelOfDetail, LoD);
   
   modelViewMatrix.pushMatrix();
   meshShader->use();
   
   glEnable(GL_TEXTURE_2D);
   
   transform(modelViewMatrix);
   meshShader->setModelMatrix(modelViewMatrix.getMatrix());
   
   
   GLuint h_aPos = meshShader->getPosLocation();
   GLuint h_aNorm = meshShader->getNormLocation();
   GLuint h_aText = meshShader->getTextLocation();
   
   safe_glEnableVertexAttribArray(h_aPos);
   glBindBuffer(GL_ARRAY_BUFFER, meshStorage[LoD].vertexBuffer);
   safe_glVertexAttribPointer(h_aPos, 3, GL_FLOAT, GL_FALSE, 0, 0);

   safe_glEnableVertexAttribArray(h_aNorm);
   glBindBuffer(GL_ARRAY_BUFFER, meshStorage[LoD].normalBuffer);
   safe_glVertexAttribPointer(h_aNorm, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   safe_glEnableVertexAttribArray(h_aText);
   glBindBuffer(GL_ARRAY_BUFFER, meshStorage[LoD].material[0].textureCoordinates);
   safe_glVertexAttribPointer(h_aText, 2, GL_FLOAT, GL_FALSE, 0, 0);

   for (int i = meshStorage[LoD].numMeshes - 1; i >= 0; i--) {
      //glActiveTexture(i);
      glBindTexture(GL_TEXTURE_2D, meshStorage[LoD].material[i].textureLocation);
      //meshShader->setTexture(meshStorage[LoD].material[i].textureLocation);
      meshShader->setMaterial(&meshStorage[LoD].material[i]);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshStorage[LoD].indexBuffer[i]);
      glDrawElements(GL_TRIANGLES, meshStorage[LoD].indexBufferLength[i], GL_UNSIGNED_SHORT, 0);
   }
   
   
   modelViewMatrix.popMatrix();
   
   if (hasShadow) {
      modelViewMatrix.pushMatrix();
      
      modelViewMatrix.translate(vec3(getPosition().x, 0.01, getPosition().z));
      modelViewMatrix.scale(scl.x, 0.001, scl.z);
      modelViewMatrix.rotate(rot.x, vec3(1.0, 0.0, 0.0));
      modelViewMatrix.rotate(rot.y, vec3(0.0, 1.0, 0.0));
      modelViewMatrix.rotate(rot.z, vec3(0.0, 0.0, 1.0));
      
      meshShader->setModelMatrix(modelViewMatrix.getMatrix());
      
      for (int i = meshStorage[LoD].numMeshes - 1; i >= 0; i--) {
         meshShader->setMaterial(&shadowMat);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshStorage[LoD].indexBuffer[i]);
         glDrawElements(GL_TRIANGLES, meshStorage[LoD].indexBufferLength[i], GL_UNSIGNED_SHORT, 0);
      }
      
      modelViewMatrix.popMatrix();
   }
   
   
   safe_glDisableVertexAttribArray(h_aPos);
   safe_glDisableVertexAttribArray(h_aNorm);
   safe_glDisableVertexAttribArray(h_aText);
   
   /*modelViewMatrix.translate(getPosition());
   modelViewMatrix.scale(scl.x, scl.y, scl.z);
   meshShader->setModelMatrix(modelViewMatrix.getMatrix());
   
   safe_glEnableVertexAttribArray(h_aNorm);
   glBindBuffer(GL_ARRAY_BUFFER, meshStorage[LoD].normalBuffer);
   safe_glVertexAttribPointer(h_aNorm, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   safe_glEnableVertexAttribArray(h_aPos);
   glBindBuffer(GL_ARRAY_BUFFER, meshStorage[LoD].shadowVBO);
   safe_glVertexAttribPointer(h_aPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   for (int i = meshStorage[LoD].numMeshes - 1; i >= 0; i--) {
      meshShader->setMaterial(&meshStorage[LoD].material[i]);
      if (meshStorage[LoD].shadowVBO != NULL) {
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshStorage[LoD].shadowIBO[i]);
         glDrawElements(GL_TRIANGLES, meshStorage[LoD].shadowBufferLength[i], GL_UNSIGNED_SHORT, 0);
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

void GameDrawableObject::onCollide(GameDrawableObject *other, float dt)
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
   
   drawable_objects.update(this);

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

int GameDrawableObject::lodIndex(float levelOfDetail)
{
   //assert(levelOfDetail > 0 && levelOfDetail <= 1);
   return floor((1-levelOfDetail) * meshStorage.size());
}

