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

GameDrawableObject::GameDrawableObject(GamePhysicsActor *actor, const char *objFile) :
   GameObject(actor)
{
   g_model_manager->getObject(objFile, &meshStorage);

#ifdef DEBUG_VBO
   printf("VBO Arrived at its Destination: %d\n", (int)indexBufferLength[0]);
#endif
   //dir = vec3(0.0, 0.0, 1.0);
   //spd = 0.0;
   
   //pos = vec3(0.0, 0.0, 5.0);
   rot = vec3(0.0, 0.0, 0.0);
   scl = vec3(1.0, 3.0, 1.0);
   
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


void GameDrawableObject::draw(FlatShader *meshShader, RenderingHelper modelViewMatrix)
{
   modelViewMatrix.pushMatrix();
   meshShader->use();
   
   modelViewMatrix.translate(position());
   modelViewMatrix.scale(scl.x, scl.y, scl.z);
   modelViewMatrix.rotate(rot.x, vec3(1.0, 0.0, 0.0));
   modelViewMatrix.rotate(rot.y, vec3(0.0, 1.0, 0.0));
   modelViewMatrix.rotate(rot.z, vec3(0.0, 0.0, 1.0));
   meshShader->setModelMatrix(modelViewMatrix.getMatrix());
   
   //glBindVertexArray(vertexArray);
   
   GLuint h_aPos = meshShader->getPosLocation();
   
   safe_glEnableVertexAttribArray(h_aPos);
   glBindBuffer(GL_ARRAY_BUFFER, meshStorage.vertexBuffer);
   safe_glVertexAttribPointer(h_aPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   for (int i = 0; i < meshStorage.numMeshes; i++) {
      //printf("We are drawing, right? %d\n", indexBufferLength[i]);
      
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshStorage.indexBuffer[i]);
   
      glDrawElements(GL_TRIANGLES, meshStorage.indexBufferLength[i], GL_UNSIGNED_SHORT, 0);
   }
   
   safe_glDisableVertexAttribArray(h_aPos);
   
   //glBindVertexArray(0);
   modelViewMatrix.popMatrix();
}
