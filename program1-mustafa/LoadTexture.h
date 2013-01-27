
#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glut.h>


/*data structure for the image used for  texture mapping */
typedef struct Image {
  unsigned long sizeX;
  unsigned long sizeY;
  char *data;
} Image;


typedef struct RGB {
  GLubyte r;
  GLubyte g; 
  GLubyte b;
} RGB;


void makeCheckerBoard(int nRows, int nCols);

GLvoid LoadTexture(const char* image_file, int texID);

GLvoid LoadTextureAlpha(const char* image_file, int texID);

int ImageLoad(const char *filename, Image *image);

int ImageLoad32bpp(const char *filename, Image *image);


#endif
