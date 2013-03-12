//
//  GameRamp.h
//  
//
//  Created by Bryan Bell on 1/30/13.
//
// NOTE: Do not rotate this object by anything but the y-axis,
//  it will not work properly if the object is not flat.

#ifndef ____GameTree__
#define ____GameTree__

#include <iostream>

#include "GameSceneryObject.h"

PhongMaterial tree_materials[2] = {
                  {vec3(0.2, 0.2, 0.2), // amb
                   vec3(H2_3f(0x6d4100)), // diff
                   vec3(0, 0, 0),       // spec
                   10.0},               // shine
                  {vec3(0.2, 0.2, 0.2), // amb
                   vec3(H2_3f(0x0d5600)), // diff
                   vec3(0.1, 0.8, 0.1),       // spec
                   20.0},               // shine
};



class GameSceneryTree : public GameSceneryObject {
public:
   GameSceneryTree() : GameSceneryObject("models/colorCoatRack.obj") {
   }
   
   float getHeightAt(float x, float z) {
       return getPosition().y + boundingInfo.dimension.y/2;
   }

    virtual void draw(PhongShader *meshShader, RenderingHelper modelViewMatrix, float levelOfDetail)
    {
       int LoD = lodIndex(levelOfDetail);
       
       modelViewMatrix.pushMatrix();
       meshShader->use();
       
       modelViewMatrix.translate(vec3(getPosition().x, getPosition().y - 0.8f * scl.y,getPosition().z));
       modelViewMatrix.scale(scl.x, scl.y, scl.z);
       modelViewMatrix.rotate(rot.x, vec3(1.0, 0.0, 0.0));
       modelViewMatrix.rotate(rot.y, vec3(0.0, 1.0, 0.0));
       modelViewMatrix.rotate(rot.z, vec3(0.0, 0.0, 1.0));
       meshShader->setModelMatrix(modelViewMatrix.getMatrix());
       
       
       GLuint h_aPos = meshShader->getPosLocation();
       GLuint h_aNorm = meshShader->getNormLocation();
       // TODO - add texture buffer
       
       safe_glEnableVertexAttribArray(h_aPos);
       glBindBuffer(GL_ARRAY_BUFFER, meshStorage[LoD].vertexBuffer);
       safe_glVertexAttribPointer(h_aPos, 3, GL_FLOAT, GL_FALSE, 0, 0);

       safe_glEnableVertexAttribArray(h_aNorm);
       glBindBuffer(GL_ARRAY_BUFFER, meshStorage[LoD].normalBuffer);
       safe_glVertexAttribPointer(h_aNorm, 3, GL_FLOAT, GL_FALSE, 0, 0);
       
       // draw leaves
       int i;
       for (i = 0; i < meshStorage[LoD].numMeshes-1; i++) {
          meshShader->setMaterial(&tree_materials[1]);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshStorage[LoD].indexBuffer[i]);
          glDrawElements(GL_TRIANGLES, meshStorage[LoD].indexBufferLength[i], GL_UNSIGNED_SHORT, 0);
       }
       // draw trunk
       meshShader->setMaterial(&tree_materials[0]);
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshStorage[LoD].indexBuffer[i]);
       glDrawElements(GL_TRIANGLES, meshStorage[LoD].indexBufferLength[i], GL_UNSIGNED_SHORT, 0);
       
       safe_glDisableVertexAttribArray(h_aPos);
       safe_glDisableVertexAttribArray(h_aNorm);
       
       modelViewMatrix.popMatrix();
    }
};

#endif /* defined(____GameBuilding__) */
