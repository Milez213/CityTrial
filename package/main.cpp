//
// main.cpp
// Progam 01
//
// Created by Bryan Bell on 1/12/13
// For CSC 476 - Real Time 3D Graphics


#ifdef __APPLE__
#include <GL/glfw.h>
#endif

#ifdef __unix__
#include "GL/glfw.h"
#endif



#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include "GLSL_helper.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "MStackHelp.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <assert.h>
#include <vector>

#include "BBCamera.h"
#include "BBPlayerObject.h"
#include "BBCollectObject.h"
#include "BBAvoidObject.h"
#include "BBLight.h"

using namespace std;
using namespace glm;

GLint h_uTexUnit;
GLint h_aPosition;
GLint h_aNormal;
GLint h_aTexCoord;
GLint h_uDiffuse;
GLint h_uSpecular;
GLint h_uShaderFlags;
GLint h_uSun;
GLint h_uCamera;
GLint h_uClosestLight;
GLint h_uModelMatrix;
GLint h_uViewMatrix;
GLint h_uProjMatrix;
GLint h_uNormalMatrix;

RenderingHelper ModelTrans;

int ShadeProg;

vector<BBAvoidObject> avoids;
vector<BBPlayerObject> players;
vector<BBCollectObject> collects;
BBMatObject *ground;
BBLight *sun;
BBCamera *camera;

float theta = 0, phi = 0;

int w_wdth, w_hgt;

double lastTime;

/* projection matrix */
void SetProjectionMatrix() {
    glm::mat4 Projection = glm::perspective(80.0f, (float)w_wdth/w_hgt, 0.1f, 100.f);
    safe_glUniformMatrix4fv(h_uProjMatrix, glm::value_ptr(Projection));
}

void SetOrthoMatrix() {
	glm::mat4 Projection = glm::ortho(0.0f, (float)w_wdth, (float)w_hgt, 0.0f, -5.0f, 5.0f);
    safe_glUniformMatrix4fv(h_uProjMatrix, glm::value_ptr(Projection));
    
   glm::mat4 identity = mat4(1.0f);
   safe_glUniformMatrix4fv(h_uViewMatrix, glm::value_ptr(identity));
   safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(identity));
}

/* camera controls - do not change */
void SetView() {
    vec3 up = glm::vec3(0.0, 1.0, 0.0);
    glm::mat4 lookAt = glm::lookAt(camera->Position(), camera->LookAt(), up);
    safe_glUniformMatrix4fv(h_uViewMatrix, glm::value_ptr(lookAt));
}

/* set the model transform to the identity */
void SetModel() {
    glm::mat4 tmp = glm::mat4(1.0f);
    safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(tmp));
    safe_glUniformMatrix4fv(h_uNormalMatrix, glm::value_ptr(tmp));
}

//-----------------------------------------------
// Initialization Functions
//-----------------------------------------------
int load_obj(const char* filename, vector<BBMatObject> *newObjects/*GeomModel models[]*/) {
    vector<glm::vec4> vertices;
    vector<glm::vec3> normals;
    vector<glm::vec2> textures;
    vector<glm::vec2> texturesTemp;
    vector<GLushort> elements;
    //vector<glm::vec2>::iterator txtIter;
    
    int objectStarted = 0, useMtl = 0;
    BBMatObject *newObject = new BBMatObject;
    ifstream in(filename, ios::in);
    if (!in) { cerr << "Cannot open " << filename << endl; exit(1); }
    
    string line;
    while (getline(in, line)) {
        if (line.substr(0,2) == "o ") {
            if (objectStarted == 1) {
                normals.resize(vertices.size(), glm::vec3(0.0, 0.0, 0.0));
                for (int i = 0; i < elements.size(); i+=3) {
                    GLushort ia = elements[i];
                    GLushort ib = elements[i+1];
                    GLushort ic = elements[i+2];
                    glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]),
                                                                 glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
                    normals[ia] += normal;
                    normals[ib] += normal;
                    normals[ic] += normal;
                }
                
                newObject->BBBindBuffers(vertices, normals, elements, textures);
                newObjects->push_back(*newObject);
                normals.clear();
                elements.clear();
                textures.clear();
                newObject = new BBMatObject;
            }
            istringstream s(line.substr(2));
            objectStarted = 1;
        }
        else if (line.substr(0,2) == "v ") {
            istringstream s(line.substr(2));
            glm::vec4 v; s >> v.x; s >> v.y; s >> v.z; v.w = 1.0f;
            vertices.push_back(v);
        }  else if (line.substr(0,2) == "f ") {
            if (useMtl == 1) {
                string s = line.substr(2);
                string n;
                int txtCrd = 0;
                GLushort a;
                
                for (int i = 0; i <= s.length(); i++) {
                    if (s[i] != 47 && s[i] != 32 && i != s.length()) {
                        n += s[i];
                    } else {
                        istringstream str(n);
                        if (txtCrd == 0) {
                            str >> a;
                            a--;
                            //printf("Vertex %d has texture coordinates ", a);
                            elements.push_back(a);
                        } else {
                            int t;
                            str >> t;
                            //printf("(%0.3f, %0.3f)\n", texturesTemp.at(t-1).x, texturesTemp.at(t-1).y);
                            if (textures.at(a).x != texturesTemp.at(t-1).x && textures.at(a).x != 0.0) {
                                //printf("Replacing existing texture at %d\n", a+1);
                            }
                            textures.at(a).x = texturesTemp.at(t-1).x;
                            textures.at(a).y = texturesTemp.at(t-1).y;
                        }
                        n.clear();
                        txtCrd = (txtCrd + 1) % 2;
                    }
                }
            } else {
                istringstream s(line.substr(2));
                GLushort a,b,c;
                s >> a; s >> b; s >> c;
                a--; b--; c--;
                elements.push_back(a); elements.push_back(b); elements.push_back(c);
            }
        } else if (line.substr(0,3) == "vt ") {
            istringstream s(line.substr(3));
            glm::vec2 v; s >> v.x; s >> v.y;
            texturesTemp.push_back(v);
        } else if (line.substr(0,6) == "usemtl") {
        		if (useMtl == 1) {
        			textures.resize(vertices.size(), vec2(0.0, 0.0));
        		}
        	
        } else if (line.substr(0,6) == "mtllib") {
        		string mtl = "models/";
        		mtl += line.substr(7);
        		if (newObject->BBLoadMaterial(mtl.c_str()) == 1) {
        			useMtl = 1;
        		} else {
        			useMtl = 0;	
        		}
        }
        else if (line[0] == '#') { /* ignoring this line */ }
        else { /* ignoring this line */ }
    }
    
    normals.resize(vertices.size(), glm::vec3(0.0, 0.0, 0.0));
    for (int i = 0; i < elements.size(); i+=3) {
 	//printf("Face %d: (%d, %d, %d)\n", i / 3, elements[i], elements[i+1], elements[i+2]);
        GLushort ia = elements[i];
        GLushort ib = elements[i+1];
        GLushort ic = elements[i+2];
        glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]),
                                                     glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
        normals[ia] += normal;
        normals[ib] += normal;
        normals[ic] += normal;
    }
    newObject->BBBindBuffers(vertices, normals, elements, textures);
    //printf("%d\n", newObject->IndexBufferLength());
    newObjects->push_back(*newObject);
    
    return 0;
}

/*function to help load the shaders (both vertex and fragment */
int InstallShader(const GLchar *vShaderName, const GLchar *fShaderName) {
    GLuint VS; //handles to shader object
    GLuint FS; //handles to frag shader object
    GLint vCompiled, fCompiled, linked; //status of shader
    
    VS = glCreateShader(GL_VERTEX_SHADER);
    FS = glCreateShader(GL_FRAGMENT_SHADER);
    
    //load the source
    glShaderSource(VS, 1, &vShaderName, NULL);
    glShaderSource(FS, 1, &fShaderName, NULL);
    
    //compile shader and print log
    glCompileShader(VS);
    /* check shader status requires helper functions */
    printOpenGLError();
    glGetShaderiv(VS, GL_COMPILE_STATUS, &vCompiled);
    printShaderInfoLog(VS);
    
    //compile shader and print log
    glCompileShader(FS);
    /* check shader status requires helper functions */
    printOpenGLError();
    glGetShaderiv(FS, GL_COMPILE_STATUS, &fCompiled);
    printShaderInfoLog(FS);
    
    if (!vCompiled || !fCompiled) {
        printf("Error compiling either shader %s or %s", vShaderName, fShaderName);
        return 0;
    }
    
    //create a program object and attach the compiled shader
    ShadeProg = glCreateProgram();
    glAttachShader(ShadeProg, VS);
    glAttachShader(ShadeProg, FS);
    
    glLinkProgram(ShadeProg);
    /* check shader status requires helper functions */
    printOpenGLError();
    glGetProgramiv(ShadeProg, GL_LINK_STATUS, &linked);
    printProgramInfoLog(ShadeProg);
    
    glUseProgram(ShadeProg);

    /* get handles to attribute data */
    h_uTexUnit = safe_glGetUniformLocation(ShadeProg, "uTexUnit");
    h_aPosition = safe_glGetAttribLocation(ShadeProg, "aPosition");
    h_aNormal = safe_glGetAttribLocation(ShadeProg,  "aNormal");
    h_aTexCoord = safe_glGetAttribLocation(ShadeProg,  "aTexCoord");    
    h_uDiffuse = safe_glGetUniformLocation(ShadeProg, "uDiffuse");
    h_uSpecular = safe_glGetUniformLocation(ShadeProg, "uSpecular");
    h_uShaderFlags = safe_glGetUniformLocation(ShadeProg, "uShaderFlags");
    h_uSun = safe_glGetUniformLocation(ShadeProg, "uSun");
    h_uCamera = safe_glGetUniformLocation(ShadeProg, "uCamera");
    h_uClosestLight = safe_glGetUniformLocation(ShadeProg, "uLight");
    h_uModelMatrix = safe_glGetUniformLocation(ShadeProg, "uModelMatrix");
    h_uViewMatrix = safe_glGetUniformLocation(ShadeProg, "uViewMatrix");
    h_uProjMatrix = safe_glGetUniformLocation(ShadeProg, "uProjMatrix");
    h_uNormalMatrix = safe_glGetUniformLocation(ShadeProg, "uNormalMatrix");
    printf("sucessfully installed shader %d\n", ShadeProg);
    return 1;
    
}

void InitGeometry()
{
	vector<BBMatObject> initialObjects;
	
	load_obj("models/player.obj", &initialObjects);
	
	for (int i = 0; i < initialObjects.size(); i++) {
		//printf("ibl: %d\n", initialObjects[i].IndexBufferLength());
		BBPlayerObject *newObject = new BBPlayerObject(initialObjects[i]);
		vec3 pos = newObject->Position();
		newObject->SetPosition(vec3(pos.x, 0.0, pos.z));
		players.push_back(*newObject);
	}
	
	initialObjects.clear();
	load_obj("models/spikey.obj", &initialObjects);
	
	for (int i = 0; i < initialObjects.size(); i++) {
		//printf("ibl: %d\n", initialObjects[i].IndexBufferLength());
		BBAvoidObject *newObject = new BBAvoidObject(initialObjects[i]);
		newObject->BBRandomPosition();
		avoids.push_back(*newObject);
	}
	
	initialObjects.clear();
	load_obj("models/starText.obj", &initialObjects);
	
	for (int i = 0; i < initialObjects.size(); i++) {
		//printf("ibl: %d\n", initialObjects[i].IndexBufferLength());
		BBCollectObject *newObject = new BBCollectObject(initialObjects[i]);
		newObject->BBRandomPosition();
		collects.push_back(*newObject);
	}
	
	initialObjects.clear();
	
	load_obj("models/floor.obj", &initialObjects);
	ground = new BBMatObject();
	ground->CopyMatObject(initialObjects[0]);
}

void Initialize()
{
	/* Start OpenGL Initialization */
   glClearColor (0.8f, 0.8f, 1.0f, 1.0f);
   // Black Background
   glClearDepth (1.0f);    // Depth Buffer Setup
   glDepthFunc (GL_LEQUAL);    // The Type Of Depth Testing
   glEnable (GL_DEPTH_TEST);// Enable Depth Testing
   /* texture specific settings */
   glEnable(GL_TEXTURE_2D);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   
   ModelTrans.useModelViewMatrix();
   ModelTrans.loadIdentity();
   /* End OpenGL Initialization */
   
   /* Start Shader Initialization */
   if (!InstallShader(textFileRead((char *)"tex_vert.glsl"), textFileRead((char *)"tex_frag.glsl"))) {
      printf("Error installing shader!\n");
      return;
   }
   /* End Shader Initialization */
   
   InitGeometry();
   
	camera = new BBCamera;
	camera->SetPosition(vec3(3.0, 3.0, 0.0));
	camera->SetLookAt(vec3(0.0, 0.0, 0.0));
	
	sun = new BBLight;
	sun->SetPosition(vec3(0.0, 50.0, 0.0));
	sun->SetIntensity(0.75);
	
	if (glfwGetJoystickParam(GLFW_JOYSTICK_1, GLFW_PRESENT) == GL_TRUE) {
		printf("Number of Axes: %d\n", glfwGetJoystickParam(GLFW_JOYSTICK_1, GLFW_AXES));
		printf("Number of Buttons: %d\n", glfwGetJoystickParam(GLFW_JOYSTICK_1, GLFW_BUTTONS));
	}
}

#define DEAD_ZONE 0.3
#define RAD 5
vec3 MoveCamera(float axes[])
{
	if (axes[1] > DEAD_ZONE || axes[1] < -DEAD_ZONE)
		theta += M_PI / 32.0 * axes[1];
	if (axes[0] > DEAD_ZONE || axes[0] < -DEAD_ZONE)
		phi += M_PI / 32.0 * axes[0];
	
	vec3 rot = players[0].Rotation();
	//printf("theta: %0.3f\n", theta);
	players[0].SetRotation(vec3(rot.x, theta * (180 / M_PI) - 42, rot.z));
	
	if (theta < 0.0)
		theta += M_PI * 2.0;
	else if (theta > M_PI * 2.0)
		theta -= M_PI * 2.0;
	
	if (phi > M_PI / 3.0)
		phi = M_PI / 3.0;
	else if (phi < M_PI / 64.0)
		phi = M_PI / 64.0;
		
	return vec3(RAD * sin(theta) * cos(phi), RAD * sin(phi), RAD * cos(theta) * cos(phi));
}

void CalculateStateChanges()
{
	double time = glfwGetTime();
	float axes[8];
	
	glfwGetJoystickPos(GLFW_JOYSTICK_1, axes, 8);
	/*for (int i = 0; i < 8; i++) {
		if (i == 7)
			printf("Axis 7: %0.3f\n", axes[7]);
		else
			printf("Axis %d: %0.3f - ", i, axes[i]);	
	}*/
	
	MoveCamera(axes);
	
	vec3 gaze = camera->Position() - camera->LookAt();
	gaze = glm::normalize(gaze);
	
	vec3 u = glm::normalize(glm::cross(vec3(0.0, 1.0, 0.0), gaze));
	//printf("u: (%0.3f, %0.3f) - gaze: (%0.3f, %0.3f)\n", u.x, u.z, gaze.x, gaze.z);
	
	/*if ((int)time % 3 == 0 && (int)time != (int)lastTime) {
		BBAvoidObject *newObject = new BBAvoidObject(avoids[0]);
		newObject->BBRandomPosition();
		avoids.push_back(*newObject);
	}*/
	
	vec4 cen1;
	vec3 pos1;
	vec4 cen2;
	vec3 pos2;
	vec3 diff;
	
	float dist;
	
	for (int i = 0; i < avoids.size(); i++) {
		for (int j = i + 1; j < avoids.size(); j++) {
			cen1 = avoids[i].Center();
			pos1 = vec3(cen1.x, cen1.y, cen1.z);
			cen2 = avoids[j].Center();
			pos2 = vec3(cen2.x, cen2.y, cen2.z);
			
			diff = pos1 - pos2;
			
			dist = sqrt((diff.x * diff.x) + (diff.z * diff.z));
			
			if (dist < cen1.w + cen2.w) {
				avoids[i].BBCollideWithSphere(avoids[j]);
				avoids[j].BBCollideWithSphere(avoids[i]);
			}
		}
	}
	
	for (int i = 0; i < avoids.size(); i++) {
		cen1 = avoids[i].Center();
		pos1 = vec3(cen1.x, cen1.y, cen1.z);
		cen2 = players[0].Center();
		pos2 = vec3(cen2.x, cen2.y, cen2.z);
		
		diff = pos1 - pos2;
		
		dist = sqrt((diff.x * diff.x) + (diff.z * diff.z));
		//printf("Distance: %0.3f\n", dist);
		
		if (dist < cen1.w + cen2.w) {
			players[0].BBCollideWithSphere(avoids[i]);
			avoids[i].BBCollideWithSphere(players[0]);
		}			
	}
	
	cen1 = collects[0].Center();
	pos1 = vec3(cen1.x, cen1.y, cen1.z);
	cen2 = players[0].Center();
	pos2 = vec3(cen2.x, cen2.y, cen2.z);
	
	diff = pos1 - pos2;
	
	dist = sqrt((diff.x * diff.x) + (diff.z * diff.z));
	//printf("Distance: %0.3f\n", dist);
	
	if (dist < cen1.w + cen2.w) {
		collects[0].BBCollideWithSphere(players[0]);
		players[0].BBCollideWithSphere(collects[0]);
		
		BBAvoidObject *newObject = new BBAvoidObject(avoids[0]);
		newObject->BBRandomPosition();
		avoids.push_back(*newObject);
		
		BBCollectObject *newCoObject = new BBCollectObject(collects[0]);
		newCoObject->BBRandomPosition();
		collects.insert(collects.begin(), *newCoObject);
	}
	
	for (int i = 0; i < players.size(); i++) {
		players[i].BBReceiveJoystick(axes, gaze, u);
		players[i].BBMove(time - lastTime);
		vec3 pos = players[i].Position();
		pos = pos + MoveCamera(axes);
		camera->SetPosition(pos);
		camera->SetLookAt(players[i].Position());
	}
	
	for (int i = 0; i < avoids.size(); i++) {
		avoids[i].BBMove(time - lastTime);
   }
   
   for (int i = 0; i < collects.size(); i++) {
		collects[i].BBMove(time - lastTime);
		if (collects[i].Position().y > 10.0)
			collects.pop_back();
   }
   lastTime = time;
}

struct RGB 
{
 unsigned char r, g, b;
};

void DrawGeometry()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
   //Start our shader
   glUseProgram(ShadeProg);
   
   //Set up matrix transforms
   SetProjectionMatrix();
   SetView();
   SetModel();
   
   glUniform4f(h_uSun, sun->Position().x, sun->Position().y, sun->Position().z, sun->Intensity());
   glUniform3f(h_uCamera, camera->Position().x, camera->Position().y, camera->Position().z);
   
   for (int i = 0; i < players.size(); i++) {
      players[i].BBDrawObject(h_aPosition, h_aNormal, h_aTexCoord, h_uDiffuse, h_uSpecular,
       h_uShaderFlags, h_uModelMatrix, h_uNormalMatrix, ModelTrans.getMatrix());
   }
  
   for (int i = 0; i < avoids.size(); i++) {
      avoids[i].BBDrawObject(h_aPosition, h_aNormal, h_aTexCoord, h_uDiffuse, h_uSpecular,
       h_uShaderFlags, h_uModelMatrix, h_uNormalMatrix, ModelTrans.getMatrix());
   }
   
   for (int i = 0; i < collects.size(); i++) {
      collects[i].BBDrawObject(h_aPosition, h_aNormal, h_aTexCoord, h_uDiffuse, h_uSpecular,
       h_uShaderFlags, h_uModelMatrix, h_uNormalMatrix, ModelTrans.getMatrix());
   }
       
   for (float i = -45; i <= 45; i += 10) {
   		for (float j = -45; j <= 45; j += 10) {
   			ground->SetPosition(vec3(i, 0.0, j));
   			ground->BBDrawObject(h_aPosition, h_aNormal, h_aTexCoord, h_uDiffuse, h_uSpecular,
       h_uShaderFlags, h_uModelMatrix, h_uNormalMatrix, ModelTrans.getMatrix());
   		} 
   } 
   
   RGB color[4];
   
   color[0].r = 255;
  	color[0].g = 0;
  	color[0].b = 0;	

  	color[1].r = 0;
  	color[1].g = 255;
  	color[1].b = 0;

  	color[2].r = 0;
  	color[2].g = 0;
  	color[2].b = 255;

  	color[3].r = 255;
  	color[3].g = 255;
  	color[3].b = 255;
  	
  	//glClear(GL_COLOR_BUFFER_BIT);
  	//glDrawPixels( 2, 2, GL_RGB, GL_UNSIGNED_BYTE, color );
  	
  	glViewport(0, 0, (GLsizei)w_wdth, (GLsizei)w_hgt);
  	glMatrixMode(GL_PROJECTION);
  	gluPerspective(60, (GLfloat)w_wdth / (GLfloat)w_hgt, 1.0, 100.0);
  	
  	glMatrixMode(GL_MODELVIEW);
  	
  	//glRasterPos2i(100, 100);
  	
  	glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations  
  
	glTranslatef(0.0f, 0.0f, -5.0f);
  	
  	glBegin(GL_QUADS);
  	glVertex3f(-1.0f, -1.0f, 0.0f); // The bottom left corner  
	glVertex3f(-1.0f, 1.0f, 0.0f); // The top left corner  
	glVertex3f(1.0f, 1.0f, 0.0f); // The top right corner  
	glVertex3f(1.0f, -1.0f, 0.0f); // Start drawing a quad primitive  
	glEnd();
	
  	glFlush();
   /*SetOrthoMatrix();*/
   
   /*float vertex[] = {
   		10.0f, 10.0f, 0.0f,
   		20.0f, 10.0f, 0.0f,
   		20.0f, 20.0f, 0.0f,
   		10.0f, 20.0f, 0.0f };
   		
   	float index[] = {
   		3, 1, 0,
   		3, 2, 1 };
   		
   	int indexLength = 6;
   	
   	GLuint vBuf, iBuf;
   	
   	glUniform4f(h_uDiffuse, 1.0, 0.0, 0.0, 1.0);
   glUniform4f(h_uShaderFlags, 0.0, 0.0, 0.0, 1.0);
   	
   	safe_glEnableVertexAttribArray(h_aPosition);
   	glGenBuffers(1, &vBuf);
   glBindBuffer(GL_ARRAY_BUFFER, vBuf);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
   safe_glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   glGenBuffers(1, &iBuf);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuf);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
       
	ModelTrans.loadIdentity();
   safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(ModelTrans.getMatrix()));
   
   //printf("Here: %d\n", indexBufferLength);
   glDrawElements(GL_TRIANGLES, indexLength, GL_UNSIGNED_SHORT, 0);
   
   safe_glDisableVertexAttribArray(h_aPosition);*/
	
	glfwSwapBuffers();
}

int main(int argc, char** argv) 
{

	w_wdth = 640;
	w_hgt = 480;
	
   if (glfwInit() == GL_TRUE)
    	glfwOpenWindow(w_wdth, w_hgt, 0, 0, 0, 0, 16, 0, GLFW_WINDOW);
    	
   Initialize();
   
   glfwSetTime(0.0);
    	
   while (glfwGetWindowParam(GLFW_OPENED)) {
    	CalculateStateChanges();
    	DrawGeometry();   	
   }
    
   glfwTerminate();
}
