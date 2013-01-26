//
// BBMatObject.h
// Progam 01
//
// Created by Bryan Bell on 1/13/13
// For CSC 476 - Real Time 3D Graphics
//
// This object defines modeled game object.

#ifndef BB_OBJECT
#define BB_OBJECT
#include "BBObject.h"
#endif

#include <vector>
#include <OpenGL/gl.h>

using namespace std;
using namespace glm;

class BBMatObject : public BBObject
{
public:
	BBMatObject();
	
	void CopyMatObject(BBMatObject object);
	
	void BBDrawObject(GLint hPos, GLint hNor, GLint hTex, GLint hDif, GLint hSpc, GLint hShd, GLint hModel, GLint hNormal, mat4 modelMatrix);
	void BBBindBuffers(vector<glm::vec4> vertices, vector<glm::vec3> normals, vector<GLushort> elements, vector<glm::vec2> textures);
	virtual void BBMove(double time) {};
	virtual void BBCollideWithSphere(BBMatObject collide) {};
	virtual void BBCollideWithBoundingBox(BBMatObject collide) {};
	
	int BBLoadMaterial(const char* fileName);
	
	//Getters and Setters
	vec3 DiffColor() { return vec3(diffColor.x, diffColor.y, diffColor.z); };
	void SetDiffColor(vec3 dClr) { diffColor = vec3(dClr.x, dClr.y, dClr.z); };
	vec3 SpecColor() { return vec3(specColor.x, specColor.y, specColor.z); };
	void SetSpecColor(vec3 sClr) { specColor = vec3(sClr.x, sClr.y, sClr.z); };
	float Diffuse() { return diffuse; };
	void SetDiffuse(float diff) { diffuse = diff; };	
	float Specularity() { return specularity; };
	void SetSpecularity(float spec) { specularity = spec; };
	
	vec4 ShaderFlags() { return vec4(shaderFlags.x, shaderFlags.y, shaderFlags.z, shaderFlags.w); };
	void SetShaderFlags(vec4 sFgs) { shaderFlags = vec4(sFgs.x, sFgs.y, sFgs.z, sFgs.w); };
	
	GLuint VertexBuffer() { return vertexBuffer; };
	void SetVertexBuffer(GLuint vbo) { vertexBuffer = vbo; };
	GLuint NormalBuffer() { return normalBuffer; };
	void SetNormalBuffer(GLuint nbo) { normalBuffer = nbo; };
	GLuint IndexBuffer() { return indexBuffer; };
	void SetIndexBuffer(GLuint ibo) { indexBuffer = ibo; };
	GLuint TextureBuffer() { return textureBuffer; };
	void SetTextureBuffer(GLuint tbo) { textureBuffer = tbo; };
	GLuint TextureLocation() { return textureLocation; };
	void SetTextureLocation(GLuint tbl) { textureLocation = tbl; };
	GLuint NormalMapBuffer() { return normalBuffer; };
	void SetNormalMapBuffer(GLuint nmbo) { normalMapBuffer = nmbo; };
	int IndexBufferLength() { return indexBufferLength; };
	void SetIndexBufferLength(int iboLength) { indexBufferLength = iboLength; };
	
	vec4 Center() { return vec4(center.x + position.x, center.y + position.y, center.z + position.z, center.w); }
	vec3 BoundingBoxMax() { return vec3(boundingBoxMax.x, boundingBoxMax.y, boundingBoxMax.z); }
	vec3 BoundingBoxMin() { return vec3(boundingBoxMin.x, boundingBoxMin.y, boundingBoxMin.z); }
	
protected:
	vec3 diffColor;
	vec3 specColor;
	float diffuse;
	float specularity;
	
	vec4 shaderFlags;
	
	vec4 center;
	vec3 boundingBoxMin;
	vec3 boundingBoxMax;
	
	GLuint textureLocation;
	
	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint indexBuffer;
	GLuint textureBuffer;
	GLuint normalMapBuffer;
	int indexBufferLength;
};