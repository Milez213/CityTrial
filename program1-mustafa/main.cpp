/*
    Mustafa Khafateh
*/

#include <iostream>

#ifdef __APPLE__
#include "GLUT/glut.h"
#include <OPENGL/gl.h>
#endif

#ifdef _WIN32
#pragma comment(lib, "glew32.lib")
#include <GL\glew.h>
#include <GL\glut.h>
#endif

#ifdef __unix__
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <list>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr

#include "GLSL_helper.h"
#include "defines.h"
#include "PhongShader.h"
#include "mesh.h"




/*
#include "RocketManager.h"
#include "PlaneManager.h"
*/

#include "Bunnies.h"

#include "LoadTexture.h"

#include "FontShader.h"


#include <ft2build.h>
#include FT_FREETYPE_H



// get components of Hex color
#define H2R(c)   (((c) >> 16) & 0xFF)/255.0
#define H2G(c)   (((c) >> 8) & 0xFF)/255.0
#define H2B(c)   ((c) & 0xFF)/255.0
#define H2_3f(c) H2R(c), H2G(c), H2B(c)


using namespace std;
using  glm::mat4;
using  glm::vec3;
using  glm::vec4;
using  glm::rotate;
using  glm::translate;
using  glm::scale;



// ======= ground =======

// Ground handles
GLuint groundVAO;
GLuint GrndBuffObj, GIndxBuffObj, GNBuffObj;
GLuint cloudTexHandle = 0;
GLuint bricksTex = 0;
GLuint bricksNormTex = 1;

int g_GiboLen;
const float g_groundY = -0.5;      // y coordinate of the ground
const float g_groundSize = 20.0;


// ======= end ground =======



// ======= fonts =======

FT_Library ft;
FT_Face face;

GLuint fontVAO, fontVBO;

FontShader *fontShader;


// ======= end fonts =======




// drawables
// VBOTorus *torus;
/*
TFBParticles *particles;
TFBParticles *parts2;
*/
Mesh *cube;
Mesh *cube2;

// rocket manager
// extern Rockets *rockets;
// Rockets *rockets;


// plane manager
// Planes *planes;

Bunnies *bunnies;

float g_width, g_height;


// camera stuff
float g_CamangleY = 0;
float g_CamangleX = 0;
float g_CamDx;
float g_CamDz;
float g_CamDy;
vec3 g_CamPos(-0.4, 0.5 ,3);

// units / second
static float moveSpeed = 2;

// +1, 0, or -1
struct objectMovement_t {
    public:
        int x;
        int z;
        int y;
    objectMovement_t() {
        x = y = z = 0;
    }
};
objectMovement_t playerDir;


int g_score = 0;


float g_time0;
float g_time;
bool g_paused = false;
float g_fps;

char xform_names[][20] = {"Object", "Light", "View", "Flame"};
enum {XT_OBJ, XT_LIGHT, XT_VIEW, XT_FLAME};
int g_xform = XT_VIEW;

// transform type
enum action_type {DO_XLATE=1, DO_SCALE, DO_ROTATE};
int g_action;

int g_oldx, g_oldy;
int g_newx, g_newy;


// model transform info
glm::vec3 g_trans(0, 0, 0);
glm::mat4 g_scaleRotMat(1.0f);
glm::vec3 g_flamePos(0, -0.5, -2);

// list of meshes
list<mat4> boxes_xt;

// lighting stuff

#define NUM_MATERIALS 4
int g_flat = 1;
int g_material = 1;
LightInfo g_lightInfo;
GLuint g_showNormals;
PhongShader *phong;

PhongMaterial g_materials[NUM_MATERIALS] = {
                  {vec3(0.2, 0.2, 0.2), // amb
                   vec3(0.7, 0.4, 0.4), // diff
                   vec3(1, 1, 1),       // spec
                   20.0},               // shine

                  {vec3(0.0, 0.0, 0.0),
                   vec3(H2_3f(0xfff852)),
                   vec3(1, 1, 1),
                   10.0},

                  {vec3(0.1, 0.1, 0.3),
                   vec3(0.6, 0.1, 0.1),
                   vec3(0.1, 0.1, 0.7),
                   20.0},
                  {vec3(0, 1, 1),  // for drawing light
                   vec3(0.0),
                   vec3(0),
                   0.0},
};




// function prototypes
void handleTransform();
void transformObject(float, float);
void transformLight(float, float);
void transformView(float, float);
void initGround();
void setModel();
void setModel(mat4 trans);
void setModelI();
void setMaterial(int i);


glm::mat4 g_proj;
glm::mat4 g_view;
glm::mat4 g_model;
vec3 g_lookAt;


// export
extern float g_time;
extern mat4 g_proj;
extern mat4 g_view;




/* projection matrix */
void setProjectionMatrix() {
  g_proj = glm::perspective(90.0f, (float)g_width/g_height, 0.1f, 100.f);
}

/* camera controls */
void setView() {
    glm::mat4 xf = glm::translate(glm::mat4(1.0f), g_CamPos);
    xf = xf * glm::rotate(glm::mat4(1), g_CamangleX, glm::vec3(1, 0, 0));
    xf = glm::rotate(xf, g_CamangleY, glm::vec3(0.0f, 1, 0));

    // FIXME
}

void setViewFP() {
    using namespace glm;

    glm::mat4 Vinv(1.0);

    // right-left rotate
    Vinv = glm::rotate(glm::mat4(1.0), g_CamangleY, glm::vec3(0.0f, 1, 0));

    Vinv = glm::rotate(Vinv, g_CamangleX, glm::vec3(1, 0, 0));

    // get lookat vec
    vec4 t = Vinv * vec4(0, 0, -1, 1);
    g_lookAt.x = t.x;
    g_lookAt.y = t.y;
    g_lookAt.z = t.z;

    // S is translation with respect to Cam
    glm::mat4 S = glm::translate(glm::mat4(1.0), glm::vec3(g_CamDx, g_CamDy, g_CamDz));

    glm::mat4 V = glm::inverse(Vinv);
    
    // S with respect to World
    vec4 camPos4f = vec4(g_CamPos.x, g_CamPos.y, g_CamPos.z, 1.0f);
    camPos4f = Vinv * S * V * camPos4f;
    g_CamPos.x = camPos4f.x;
    // == No player y movement
    // g_CamPos.y = camPos4f.y;
    g_CamPos.z = camPos4f.z;

    // printf("g_CamPos: %f, %f, %f\n", g_CamPos.x, g_CamPos.y, g_CamPos.z);

    // xlate camera in world
    Vinv =  translate(mat4(1), g_CamPos) * Vinv;

    //g_CamDz = g_CamDx = 0;

    g_view = inverse(Vinv);
}


/* model transforms */
void setModel() {
    g_model = glm::translate( glm::mat4(1.0f), g_trans) * g_scaleRotMat;
    phong->setModelMatrix(g_model);
}

void setModel(glm::mat4 trans) {
    phong->setModelMatrix(trans);
} 

/* set the model transform to the identity */
void setModelI() {
  phong->setModelMatrix(mat4(1.0f));
}


/* helper function to set up material for shading */

void setMaterial(int i) {
    if ((i >= 0) && i < NUM_MATERIALS) {
        phong->setMaterial(g_materials[i]);
    }
}



int initfont(char* fontfilename) {
     
    // DejaVuSans.ttf

    // from opengl programming on wikibooks.org

    /* Initialize the FreeType2 library */
    if (FT_Init_FreeType(&ft)) {
        fprintf(stderr, "Could not init freetype library\n");
        return 0;
    }

    /* Load a font */
    if (FT_New_Face(ft, "DejaVuSans.ttf", 0, &face)) {
        fprintf(stderr, "Could not open font %s\n", fontfilename);
        return 0;
    }



    FT_Set_Pixel_Sizes(face, 0, 48);


    glGenBuffers(1, &fontVBO);

    fontShader = new FontShader();

    return 0;
}


// == initialize ====
// Some OpenGL initialization
void initialize()
{
    // Start Of User Initialization
    // glClearColor (0.6f, 1.0f, 0.8f, 1.0f);								
    // Black Background
    int clearColor = 0x130374;

    clearColor = 0x79a4e7; // sky blue

    // glClearColor(H2R(clearColor), H2G(clearColor), H2R(clearColor), 1.0f);
    // glClearColor(H2_3f(clearColor), 1.0);
    glClearColor(H2_3f(clearColor), 1.0);

    //glClearColor (0.0f, 0.0f, 0.0f, 1.0f);								
    glClearDepth (1.0f);	// Depth Buffer Setup
    glDepthFunc (GL_LEQUAL);	// The Type Of Depth Testing
    glEnable (GL_DEPTH_TEST);// Enable Depth Testing

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_PROGRAM_POINT_SIZE);
    // glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);


    glPointSize(1);

    // mouse
    glutWarpPointer(g_width/2, g_height/2);
    glutSetCursor(GLUT_CURSOR_NONE);

    // keyboard
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

    // state
    g_action = DO_XLATE;

    g_lightInfo.pos = glm::vec3(1, 0, 1);
    g_lightInfo.color = glm::vec3(1.0f, 1.0f, 1.0f);

    g_materials[3].aColor = g_lightInfo.color;

    g_newx = g_oldx = g_width/2;
    g_newy = g_oldy = g_height/2;
}



// == initGeom ===
// initialize the geomtry (including color)
// Change file name to load a different mesh file

void initGeom() {

    phong = new PhongShader();

    initGround();

    // R, r, sides, rings
    // torus = new VBOTorus(4, 2, 20, 10);

    cube = new Mesh("cube.m");

    cube2 = new Mesh("cube.m");
    
    /*
    particles = new TFBParticles();
    particles->init(vec3(g_flamePos));

    parts2 = new TFBParticles();
    parts2->init(g_lightInfo.pos);
    */

    // rockets = new Rockets(phong);
    // planes = new Planes(phong, rockets);

    bunnies = new Bunnies(phong);

    LoadTexture("grass.bmp", bricksTex);
    // LoadTextureAlpha("smoke32.bmp", 1);


    initfont(NULL);

    // LoadTexture("bricks-norm.bmp", bricksNormTex);
}



void initGround() {

  // A x-z plane at y = g_groundY of dimension [-g_groundSize, g_groundSize]^2
    float GrndPos[] = { 
    -g_groundSize, g_groundY, -g_groundSize, 
    -g_groundSize, g_groundY,  g_groundSize, 
     g_groundSize, g_groundY,  g_groundSize, 
     g_groundSize, g_groundY, -g_groundSize
    };

    float GrndNorm[] = { 
     0, 1, 0, 
     0, 1, 0, 
     0, 1, 0, 
     0, 1, 0, 
    };

    unsigned short idx[] = {0, 2, 1, 0, 3, 2};
    
    g_GiboLen = 6;
    
    // position
    glGenBuffers(1, &GrndBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, GrndBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GrndPos), GrndPos, GL_STATIC_DRAW);
    
    // normals
    glGenBuffers(1, &GNBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, GNBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GrndNorm), GrndNorm, GL_STATIC_DRAW);

    // index array
    glGenBuffers(1, &GIndxBuffObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

    // setup ground vertex array
    glGenVertexArrays(1, &groundVAO);
    glBindVertexArray(groundVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, GrndBuffObj);
    glVertexAttribPointer(POS_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(POS_LOCATION);

    glBindBuffer(GL_ARRAY_BUFFER, GNBuffObj);
    glVertexAttribPointer(NORM_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(NORM_LOCATION);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);

    glBindVertexArray(0);


}

struct point {
	GLfloat x;
	GLfloat y;
	GLfloat s;
	GLfloat t;
};


/**
 * from opengl programming on wikibooks.org
 *
 * Render text using the currently loaded font and currently set font size.
 * Rendering starts at coordinates (x, y), z is always 0.
 * The pixel coordinates that the FreeType2 library uses are scaled by (sx, sy).
 */
void render_text(const char *text, float x, float y, float sx, float sy) {
	const char *p;
	FT_GlyphSlot g = face->glyph;

	/* Create a texture that will be used to hold one "glyph" */
	GLuint tex;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	// glUniform1i(uniform_tex, 0);
    fontShader->setTexture(0);


	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    GLint attribute_coord = fontShader->getAtribCoord();

	/* Set up the VBO for our vertex data */
	glEnableVertexAttribArray(attribute_coord);
	glBindBuffer(GL_ARRAY_BUFFER, fontVBO);
	glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);

	/* Loop through all characters */
	for (p = text; *p; p++) {
		/* Try to load and render the character */
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;

		/* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		/* Calculate the vertex and texture coordinates */
		float x2 = x + g->bitmap_left * sx;
		float y2 = -y - g->bitmap_top * sy;
		float w = g->bitmap.width * sx;
		float h = g->bitmap.rows * sy;

		point box[4] = {
			{x2, -y2, 0, 0},
			{x2 + w, -y2, 1, 0},
			{x2, -y2 - h, 0, 1},
			{x2 + w, -y2 - h, 1, 1},
		};

		/* Draw the character on the screen */
		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		/* Advance the cursor to the start of the next character */
		x += (g->advance.x >> 6) * sx;
		y += (g->advance.y >> 6) * sy;
	}

	glDisableVertexAttribArray(attribute_coord);
	glDeleteTextures(1, &tex);
}


void drawTexts() {

    float sx = 2.0 / glutGet(GLUT_WINDOW_WIDTH);

    float sy = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);


    fontShader->use();


    /* Enable blending, necessary for our alpha texture */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    GLfloat black[4] = { 0, 0, 0, 1 };
    GLfloat red[4] = { 1, 0, 0, 1 };
    GLfloat transparent_green[4] = { 0, 1, 0, 0.5 };

    /* Set font size to 48 pixels, color to black */

    FT_Set_Pixel_Sizes(face, 0, 14);


    // glUniform4fv(uniform_color, 1, black);


    /* Effects of alignment */
    // render_text("The Quick Brown Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 50 * sy, sx, sy);
    
    char fpsstr[100];
    sprintf(fpsstr, "fps: %0.2f", g_fps);
    
    render_text(fpsstr, -1 + 8 * sx, 1 - 50 * sy, sx, sy);

    sprintf(fpsstr, "score: %d", g_score);
    render_text(fpsstr, -1 + 8 * sx, 1 - 30 * sy, sx, sy);

    sprintf(fpsstr, "bunnies: %d", bunnies->get_num_bunnies());
    render_text(fpsstr, -1 + 8 * sx, 1 - 70 * sy, sx, sy);

    // TODO incoporate into other code
    // glutSwapBuffers();

}


void drawGround() {
    // draw!
    glBindVertexArray(groundVAO);
    glDrawElements(GL_TRIANGLES, g_GiboLen, GL_UNSIGNED_SHORT, 0);
}

void drawMesh() {

    /*
    Drawable *d;
    d = cube;
    d->render();
    */

    cube->render();

}



// == Draw =====
// Main display function
void Draw(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setProjectionMatrix();
    setViewFP();

    // *** begin mesh shader
    phong->use();
    phong->setProjMatrix(g_proj);
    phong->setViewMatrix(g_view);
    phong->setCamPos(g_CamPos);

    // Light info
    phong->setLight(g_lightInfo);
    phong->setShowNormals(g_showNormals);

    // --- draw mesh

    /*
    setMaterial(g_material);
    setModel();
    drawMesh();

    setModel(translate(mat4(1.0f), vec3(2, 0, 0)));
    drawMesh();
    */


    phong->use();

    // draw boxes

    /*
    list<mat4>::iterator it;
    for (it=boxes_xt.begin(); it != boxes_xt.end(); it++) {
        setModel(*it);
        drawMesh();
    }
    */

    // --- end draw meshes

    // --- draw light ---
    setMaterial(3);
    mat4 sc = glm::scale(mat4(1.0f), vec3(0.1f));
    mat4 xl = translate(mat4(1.0f), g_lightInfo.pos);
    setModel(xl * sc);
    cube2->render();
    // -- end light
    

    // draw ground

    // ground texture
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bricksTex);
    phong->setTexture(bricksTex);

    setModelI();
    setMaterial(2);
    drawGround();

    /*
    // --- render particles
        particles->use();
        particles->setProjMatrix(g_proj);
        particles->setViewMatrix(g_view);

        // setModel();
        particles->setModelMatrix(mat4(1.0f));
        particles->setPos(g_flamePos);
        particles->render();

        //particles->setPos(vec3(0, 2, 0));
        //particles->render();
    // --- end render particles

    // --- parts2
        parts2->use();
        parts2->setProjMatrix(g_proj);
        parts2->setViewMatrix(g_view);

        // setModel();
        parts2->setModelMatrix(mat4(1.0f));
        parts2->setPos(g_lightInfo.pos);
        parts2->render();
    // --- end parts2
    */

    setMaterial(0);
    // rockets->render();


    bunnies->render();

    // printOpenGLError();

    glUseProgram(0);


    drawTexts();

    glutSwapBuffers();
}


float maxV = 2;
float currentV;

float a = 4;


void updateScene(float t, float dt) {

    // rockets->update(t, dt);
    // planes->update(t, dt);

    bunnies->update(t, dt, g_CamPos, &g_score);


    /*
    particles->update((float) t * MS_2_S);
    parts2->update((float) t * MS_2_S);
    */

    // g_trans = glm::vec3(glm::cos(light_th), glm::sin(light_th), 0);

    /*
    g_scaleRotMat = glm::rotate(mat4(1.0f), 100 * dt*MS_2_S, vec3(0, 1, 0))
            * g_scaleRotMat;
    */

    // deaccelerate
    /*
    if (playerDir.x != 0) {
        currentV = maxV;
    }

    if (playerDir.x == 0 && currentV > 0) {
        currentV -= a * dt * MS_2_S;
        printf("v: %f\n", currentV);
        g_CamDx = currentV * dt * MS_2_S;
    }

    */

    g_CamDx = playerDir.x * moveSpeed * dt * MS_2_S;
    g_CamDz = playerDir.z * moveSpeed * dt * MS_2_S;
    g_CamDy = playerDir.y * moveSpeed * dt * MS_2_S;

}

void GameLoop(void) {
    int dt;

    if (!g_paused) {
        g_time = glutGet(GLUT_ELAPSED_TIME);
        dt = g_time - g_time0;

        
        if (dt != 0) {
            // cout << "fps: " << 1.0/(dt * MS_2_S) << endl;
            g_fps =  1.0 / (MS_2_S * (g_time - g_time0)); //  1.0/(dt * MS_2_S);
        }
        
        g_time0 = g_time;

        updateScene(g_time, dt);

        Draw();
    }
}

/* Reshape */
void ReshapeGL (int width, int height)
{
    g_width = (float)width;
    g_height = (float)height;
    glViewport (0, 0, (GLsizei)(width), (GLsizei)(height));

}

// pixel coords to window coords
float p2wx(int in_x) {
  //fill in with the correct return value
  return ((2.0 / (g_width - 1)) * in_x) - 1;
}


float p2wy(int in_y) {
  // flip glut's y
  in_y = g_height - in_y - 1;

  // fill in with the correct return value
  return ((2.0 / (g_height - 1)) * in_y) - 1;;
}


void transformFlame(float dx, float dy) {
    g_flamePos += glm::vec3(dx, 0, -dy);
}

void handleTransform() {
    float dx, dy;

    if ((g_newx == g_width/2 && g_newy == g_height/2) ||
        ((g_newx == g_oldx) && (g_newy == g_oldy)))
        return;

    dx = p2wx(g_newx);
    dy = p2wy(g_newy);

    switch(g_xform) {
    case XT_VIEW:
        transformView(dx, dy);
        break;
    case XT_LIGHT:
        transformLight(dx, dy);
        break;
    case XT_OBJ:
        transformObject(dx, dy);
        break;
    case XT_FLAME:
        transformFlame(dx, dy);
        break;
    }

    glutWarpPointer(g_width/2, g_height/2);
}



void transformView(float x1, float y1) {

    // adjust by dt. fixes lag on my laptop
    // float dt = glutGet(GLUT_ELAPSED_TIME) - g_time0;

    // printf("dt: %f\n", dt);

    g_CamangleY -= 50 * (x1);
    g_CamangleX += 50 * (y1);

    
    /*
    cout << "cx: " << g_CamangleX << ", cy: " << g_CamangleY << endl;
    printf("cam pos: %f %f %f\n", g_CamPos.x, g_CamPos.y, g_CamPos.z);
    */
}

void transformLight(float x1, float y1) {
    g_lightInfo.pos += glm::vec3(x1, 0, -y1);
}


// reads mouse coords and populates transform matrices
void transformObject(float x1, float y1) {
    glm::vec3 u;
    glm::vec3 v;
    float s;
    float x0, y0;
    y0 = x0 = 0;

    // FIXME - should happen with respect to view coords
    switch (g_action) {
    case DO_XLATE:
        g_trans += glm::vec3(1.5*(x1), 0, - 1.5*(y1));
        break;
    case DO_SCALE:
        s = x1-x0 + y1-y0;
        g_scaleRotMat = glm::scale(g_scaleRotMat, glm::vec3(s + 1));
        break;
    case DO_ROTATE:
        float r0 = sqrt(x0*x0 + y0*y0);
        float r1 = sqrt(x1*x1 + y1*y1);
        
        if (r0 < 1) {
            u.x = x0;
            u.y = y0;
            u.z = sqrt(1 - x0*x0 - y0*y0);
        } else {
            u.x = x0/r0;
            u.y = y0/r0;
            u.z = 0;
        }

        if (r1 < 1) {
            v.x = x1;
            v.y = y1;
            v.z = sqrt(1 - x1*x1 - y1*y1);
        } else {
            v.x = x1/r1;
            v.y = y1/r1;
            v.z = 0;
        }

        glm::vec3 rot = glm::cross(u, v);
        float angle = (180/PI) * acos(glm::dot(u, v));

        glm::vec4 worldRot = g_scaleRotMat * glm::vec4(rot.x,
                                                       rot.y,
                                                       rot.z, 0);

        if (angle > 0.05)
            g_scaleRotMat =  glm::rotate(glm::mat4(1), angle,
                          rot) * g_scaleRotMat;

    break;
    }
}


void keyboardUp(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        playerDir.z = 0;
        break;
    case 's':
        playerDir.z = 0;
        break;
    case 'a':
        playerDir.x = 0;
        break;
    case 'd':
        playerDir.x = 0;
        break;
    case ' ':
        playerDir.y = 0;
        break;
    case 'z':
        playerDir.y = 0;
        break;
    default:
        break;
    }
}



void specialKeyboard(int key, int x, int y ) {

}

void keyboard(unsigned char key, int x, int y ){

    switch( key ) {

    case 'b':
        // rockets->boomAll();
        break;
    
    case 'p':
        g_paused = !g_paused;
        
        if (g_paused) {
            glutSetCursor(GLUT_CURSOR_INHERIT);
        } else {
            glutSetCursor(GLUT_CURSOR_NONE);
        }

        break;
    // config
    case 'n':
        g_showNormals = !g_showNormals;
        break;
    case 'f':
        /*
        if (g_flat) {
            cNormBuffObj = normPVBuffObj;
        } else {
            cNormBuffObj = normPFBuffObj;
        }
        */

        g_flat = !g_flat;
        break;
    // swith material
    case 'm':
        g_material++;
        g_material = g_material % 3;
        break;

    // what to transform
    case 'c':
        g_xform++;
        g_xform = (g_xform) % 4;
        
        if (g_xform == XT_VIEW) {
            glutSetCursor(GLUT_CURSOR_NONE);
        }

        cout << "Xform " << xform_names[g_xform] << endl;
        break;

    /* WASD keyes affect view/camera transform */
    case 'w':
        playerDir.z = -1;

        break;
    case 's':
        if (g_xform == XT_OBJ) {
            g_action = DO_SCALE;
        }

        playerDir.z = 1;
        break;

    case 'a':
        playerDir.x = -1;

        break;
    case 'd':
        playerDir.x = 1;
        break;

    case ' ':
        playerDir.y = 1;
        break;
    case 'z':
        playerDir.y = -1;
        break;

    case 't':
        if (g_xform == XT_OBJ) {
            g_action = DO_XLATE;
        } 
        break;

    // reset
    case 'r':
        if (g_xform == XT_OBJ) {
            g_action = DO_ROTATE;
        } else {
            g_trans = glm::vec3(0);
            g_scaleRotMat = glm::mat4(1.0f);
        }
        break;
    case 'q': case 'Q' :
        exit( EXIT_SUCCESS );
        break;
    }
}



void mouse(int button, int state, int x, int y) {
  /* if the left button is clicked */

    // printf("MouseMove(%d, %d)\n", x, y);

    // beginning of move
    if (button == GLUT_LEFT_BUTTON ) {
        if (state == GLUT_DOWN) {
            g_newx = g_oldx = x;
            g_newy = g_oldy = y;
            //glutSetCursor(GLUT_CURSOR_NONE);

            if (g_xform == XT_OBJ) {
                cout << "put" << endl;
                boxes_xt.push_back(translate(mat4(1.0f), g_trans) * g_scaleRotMat);
            }

            // rockets->add(g_CamPos, g_lookAt);
                        
        }
        if (state == GLUT_UP) {
            //glutSetCursor(GLUT_CURSOR_INHERIT);
            g_newx = g_oldx;
            g_newy = g_oldy;
        }
    }
}



void mousePassiveMotion(int x, int y) {
    g_newx = x;
    g_newy = y;

    handleTransform();

}



void mouseMove(int x, int y) {

    if ((x < g_width) && (x > 0) && (y < g_height) && (y > 0)
            && ((g_newx != x) || (g_newy != y))) {
        g_oldx = g_newx;
        g_oldy = g_newy;

        g_newx = x;
        g_newy = y;
    }
    glutWarpPointer(g_width/2, g_height/2);
}



void mainMenu(int value) {
    g_action = value;
}

void doMenus() {
    glutCreateMenu(mainMenu);
    glutAddMenuEntry("Translate", DO_XLATE);
    glutAddMenuEntry("Scale", DO_SCALE);
    glutAddMenuEntry("Rotate", DO_ROTATE);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}



int main(int argc, char *argv[]) {
    g_width = 640;
    g_height = 480;

    glutInit( &argc, argv );
    glutInitWindowPosition( 0, 0 );
    glutInitWindowSize( g_width, g_height );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutCreateWindow("Bunnies");
    glutReshapeFunc( ReshapeGL );
    glutDisplayFunc( Draw );
    glutIdleFunc( GameLoop );
    glutKeyboardFunc( keyboard );
    glutKeyboardUpFunc( keyboardUp );

	glutMouseFunc( mouse );
	glutMotionFunc( mouseMove );
    glutPassiveMotionFunc( mousePassiveMotion );

    // doMenus();

#ifdef _WIN32 
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr << "Error initializing glew! " << glewGetErrorString(err) << std::endl;
        return 1;
    }
#endif

    initialize();

    //test the openGL version
    cout << "=== version ===" << endl;
    getGLversion();
    cout << "=== end version ===" << endl;

    cout << endl;


    std::string fname;
    if (argc > 1) {
        fname = argv[1];
    } else {
        fname = "cube.m";
    }

    initGeom();
    
    glutMainLoop();
    return 0;
}



