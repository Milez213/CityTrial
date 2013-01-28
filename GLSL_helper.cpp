/*
 *  GLSL_helper.cpp
 *  CSC473
 *
 *  Many useful helper functions for GLSL shaders - gleaned from various sources including orange book
 *  Created by zwood on 2/21/10.
 *
 */
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include "GLSL_helper.h"

using namespace std;


/*function to help load the shaders (both vertex and fragment */
bool installShader(const char *vShaderFName, const char *fShaderFName,
                   GLuint &ShadeProg) {

    const GLchar *vShaderStr = textFileRead((char*) vShaderFName);
    const GLchar *fShaderStr = textFileRead((char*) fShaderFName);

    if (vShaderStr == NULL || fShaderStr == NULL) {
        return false;
    }


    GLuint VS; //handles to shader object
    GLuint FS; //handles to frag shader object
    GLint vCompiled, fCompiled, linked; //status of shader

    VS = glCreateShader(GL_VERTEX_SHADER);
    FS = glCreateShader(GL_FRAGMENT_SHADER);

    //load the source
    glShaderSource(VS, 1, &vShaderStr, NULL);
    glShaderSource(FS, 1, &fShaderStr, NULL);

    //compile vertex shader and print log
    glCompileShader(VS);
    /* check shader status requires helper functions */
    printOpenGLError();
    glGetShaderiv(VS, GL_COMPILE_STATUS, &vCompiled);

    cout << "Vertex ";
    printShaderInfoLog(VS);

    if (!vCompiled) {
        printf("Error compiling vertex shader '%s'\n", vShaderFName);
    }

    //compile frag shader and print log
    glCompileShader(FS);
    /* check shader status requires helper functions */
    printOpenGLError();
    glGetShaderiv(FS, GL_COMPILE_STATUS, &fCompiled);

    cout << "Frag ";
    printShaderInfoLog(FS);

    if (!vCompiled) {
        printf("Error compiling fragment shader '%s'\n", fShaderFName);
    }

    if (!vCompiled || !fCompiled) {
        return false;
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
    
    // TODO - check linked status

    printf("sucessfully installed shaders '%s' and '%s'\n=================\n",
            vShaderFName, fShaderFName);
    return true;
}


void checkGlErrors() {
  const GLenum errCode = glGetError();

  if (errCode != GL_NO_ERROR) {
    string error("GL Error: ");
    error += reinterpret_cast<const char*>(gluErrorString(errCode));
    cerr << error << endl;
    throw runtime_error(error);
  }
}


int printOglError (const char *file, int line) {
	/* Returns 1 if an OpenGL error occurred, 0 otherwise. */
	GLenum glErr;
	int    retCode = 0;
	
	glErr = glGetError ();
	while (glErr != GL_NO_ERROR)
    {
		printf ("glError in file %s @ line %d: %s\n", file, line, gluErrorString (glErr));
		retCode = 1;
		glErr = glGetError ();
    }
	return retCode;
}

void printShaderInfoLog (GLuint shader)
{
	GLint     infologLength = 0;
	GLint     charsWritten  = 0;
	GLchar *infoLog;
	
	printOpenGLError ();  // Check for OpenGL errors
	glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &infologLength);
	printOpenGLError ();  // Check for OpenGL errors
	
	if (infologLength > 0)
    {
		infoLog = (GLchar *)malloc (infologLength);
		if (infoLog == NULL)
        {
			puts ("ERROR: Could not allocate InfoLog buffer");
			exit (1);
        }
		glGetShaderInfoLog (shader, infologLength, &charsWritten, infoLog);
		printf ("Shader InfoLog:\n%s----\n", infoLog);
		free (infoLog);
    }
	printOpenGLError();  // Check for OpenGL errors
}

/* Print out the information log for a program object */
void printProgramInfoLog (GLuint program)
{
	GLint     infologLength = 0;
	GLint     charsWritten  = 0;
	GLchar *infoLog;
	
	printOpenGLError ();  // Check for OpenGL errors
	glGetProgramiv (program, GL_INFO_LOG_LENGTH, &infologLength);
	printOpenGLError ();  // Check for OpenGL errors
	
	if (infologLength > 0)
    	{
		infoLog = (GLchar *)malloc (infologLength);
		if (infoLog == NULL)
        	{
			puts ("ERROR: Could not allocate InfoLog buffer");
			exit (1);
        	}
		glGetProgramInfoLog (program, infologLength, &charsWritten, infoLog);
		printf ("Program InfoLog:\n%s----\n", infoLog);
		free (infoLog);
    	}
	printOpenGLError ();  // Check for OpenGL errors
}

//A helper routine to make it easier to set uniform variables in the shader
GLint getUniLoc(GLuint program, const GLchar *name) {
	GLint loc;
	
	loc = glGetUniformLocation(program, name);
	
	if (loc ==1) {
		printf("No such uniform named %s\n", name);
	}
	
	printOpenGLError();
	return loc;
}

void getGLversion() {
    GLint major, minor;

    major = minor = 0;

    // From GLSL 4.0 Cookbook
    const GLubyte *renderer = glGetString( GL_RENDERER );
    const GLubyte *vendor = glGetString( GL_VENDOR );
    const GLubyte *version = glGetString( GL_VERSION );
    const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    printf("GL Vendor: %s\n", vendor);
    printf("GL Renderer: %s\n", renderer);
    printf("GL Version (string): %s\n", version);
    printf("GL Version (integer): %d.%d\n", major, minor);
    printf("GLSL Version: %s\n", glslVersion);


    if (major < 3) {
        printf("This program requires at least GLSL 3.3\n");
      //  exit(0);
    }
}

// textfile.cpp
//
// simple reading and writing for text files
//
// www.lighthouse3d.com
//
// You may use these functions freely.
// they are provided as is, and no warranties, either implicit,
// or explicit are given
//////////////////////////////////////////////////////////////////////

char *textFileRead(char *fn) {
	FILE *fp;
	char *content = NULL;
	
	int count=0;
	
	if (fn != NULL) {
		fp = fopen(fn,"rt");
		
		if (fp != NULL) {
			
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);
			
			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

int textFileWrite(char *fn, char *s) {
	
	FILE *fp;
	int status = 0;
	
	if (fn != NULL) {
		fp = fopen(fn,"w");
		
		if (fp != NULL) {
			
			if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
				status = 1;
			fclose(fp);
		}
	}
	return(status);
}



// from stackoverflow.com. Only works when there's no frag shader
void usePointSprites() {
    const GLint texWidth = 256;
    const GLint texHeight = 256;
    const float texHalfWidth = texWidth/2.0;
    const float texHalfHeight = texHeight/2.0;

    unsigned char* pData = new unsigned char[texWidth*texHeight*4];
    for(int y=0; y<texHeight; ++y){
        for(int x=0; x<texWidth; ++x){
            int offs = (x + y*texWidth) * 4;
            float xoffs = ((float)x - texHalfWidth) / texHalfWidth;
            float yoffs = ((float)y - texHalfWidth) / texHalfHeight;
            float alpha = 1.0f - std::sqrt(xoffs*xoffs + yoffs*yoffs);
            if(alpha < 0.0f)
                alpha = 0.0f;
            pData[offs + 0] = 255; //r
            pData[offs + 1] = 0; //g
            pData[offs + 2] = 0; //b
            pData[offs + 3] = 255.0f * alpha; // * 
            //printf("alpha: %f\n", pData[x + y*texWidth + 3]);
        }
    }

    GLuint texture_name;

    glGenTextures(1, &texture_name);
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_name);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);
    glEnable(GL_POINT_SPRITE);
    glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glPointSize(32.0f);


    /*
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_DST_COLOR);
    // glDisable(GL_DEPTH_TEST);
    */

    glEnable(GL_POINT_SMOOTH);
}

