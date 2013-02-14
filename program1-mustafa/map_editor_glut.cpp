/*
    Mustafa Khafateh




    map editor for kpp


    How to use

    Move mode: in first-person: a w s d, space, z, and mouse

    Press 'c' to toggle transform object and first-person mode

    t   - translate
    s   - scale
    r   - rotate

    note that you can still move while in transform mode.

    press , and . (the keys with < and >) to toggle object.
    press j and k to toggle material (doesn't get set in kpp now.)

    o - save map.  Ctrl-Tab to the console and enter map name.

    Right now there's 2 object types: ramps and buildings.
    
    
    load map to edit: ./<program name> <mapname>

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
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr

#include "GLSL_helper.h"
#include "defines.h"
#include "PhongShader.h"
#include "LoadTexture.h"
#include "FontShader.h"

#include <ft2build.h>
#include FT_FREETYPE_H


#include "../GameDrawableObject.h"
#include "../ModelManager.h"


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
const float g_groundY = 0.0f;      // y coordinate of the ground
const float g_groundSize = 100.0;


// ======= end ground =======



// ======= fonts =======

FT_Library ft;
FT_Face face;

GLuint fontVAO, fontVBO;

FontShader *fontShader;


// ======= end fonts =======


float g_width, g_height;


// camera stuff
float g_CamangleY = 0;
float g_CamangleX = 0;
float g_CamDx;
float g_CamDz;
float g_CamDy;
vec3 g_CamPos(-0.4, 2 ,3);

// units / second
static float moveSpeed = 10;

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

char xform_names[][20] = {"Object", "View", "Light", "Flame"};
enum {XT_OBJ, XT_VIEW, XT_LIGHT, XT_FLAME};
int g_xform = XT_VIEW;

// transform type
enum action_type {DO_XLATE=1, DO_SCALE, DO_ROTATE};
int g_action;


enum translate_direction {XT_Y=0, XT_XZ};
int g_translate_direction = XT_XZ;

int g_oldx, g_oldy;
int g_newx, g_newy;


// model transform info
glm::mat4 g_scaleRotMat(1.0f);
glm::vec3 g_flamePos(0, -0.5, -2);

glm::mat4 g_ground_xform(1.0f);





// ===== Export Objects =====
struct ExportObject {
    string name;
    mat4 xt;
    int materialIndex;
    vec3 t;
    vec3 s;
    vec3 r;

    GameDrawableObject *model;
};


ModelManager *g_model_manager;

list<ExportObject> g_export_objects;
vector<ExportObject> g_object_palette;

vec3 g_export_trans(0, 0, 0);
vec3 g_export_scale(1.0f);
vec3 g_export_rot_vec3;

// index into materials
int g_current_material = 0;

// index into g_object_palette
int g_current_object = 0;

GameDrawableObject *g_cube;


void toggleObjects(int pos) {

    if (pos == -1 && g_current_object == 0) {
        g_current_object = g_object_palette.size() - 1;
    } else {
        g_current_object = (g_current_object + pos) % g_object_palette.size();
    }

    // reset -- edited for use in drawing
    g_object_palette[g_current_object].model->setScale(vec3(1.0f));
    bound boundBox =
        g_object_palette[g_current_object].model->getBoundingInfo();

    
    // place object above ground. 0.01 so it won't 100% touch
    g_export_trans.y = boundBox.dimension.y / 2 + 0.01;
    g_export_scale = vec3(1.0f);
    g_export_rot_vec3 = vec3(0.0f);
}


void printMat4(mat4 mat, FILE *file) {
    for (int i=0; i<4; i++)
    for (int j=0; j<4; j++) {
        fprintf(file, "%f%s", mat[i][j], j==3? "\n" : " ");
    };
}

void readMat4(mat4 &mat, FILE *file) {
    float tmp;
    for (int i=0; i<4; i++)
    for (int j=0; j<4; j++) {
        fscanf(file, "%f ", &tmp);
        mat[i][j] = tmp;
    };
}

void  fprint3f(vec3 v, FILE *file) {
    fprintf(file, "%f %f %f\n", v.x, v.y, v.z);
}

void  read3f(vec3 &v, FILE *file) {
    fscanf(file, "%f %f %f ", &v.x, &v.y, &v.z);
}



// lighting stuff

int g_flat = 1;
int g_material = 1;
LightInfo g_lightInfo;
GLuint g_showNormals;
PhongShader *phong;


#include "materials.h"


void toggleMaterials(int pos) {
    printf("tmb: %d\n", g_current_material);

    if (pos == -1 && g_current_material == 0) {
        g_current_material = NUM_MATERIALS - 1;
    } else if (pos == MAGIC_MATERIAL) {
            
    } else {
        g_current_material = (g_current_material + pos) % NUM_MATERIALS;
    }
    printf("tma: %d\n\n", g_current_material);
}


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

bool exportMap(const char *filename);


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
  g_proj = glm::perspective(90.0f, (float)g_width/g_height, 0.1f, 250.f);
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
    g_CamPos.y = camPos4f.y;
    g_CamPos.z = camPos4f.z;

    // printf("g_CamPos: %f, %f, %f\n", g_CamPos.x, g_CamPos.y, g_CamPos.z);

    // xlate camera in world
    Vinv =  translate(mat4(1), g_CamPos) * Vinv;

    //g_CamDz = g_CamDx = 0;

    g_view = inverse(Vinv);
}


/* model transforms */
void setModel() {
    g_model = glm::translate( glm::mat4(1.0f), g_export_trans) * g_scaleRotMat;
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

int psuedo_rand = 0;

void setMaterial(int i) {
    if ((i >= 0) && i < NUM_MATERIALS) {
        phong->setMaterial(g_materials[i]);
    } else if (i == MAGIC_MATERIAL) {
        phong->setMaterial(g_materials[psuedo_rand++ % NUM_MATERIALS]);
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

    g_lightInfo.pos = glm::vec3(1, 10, 1);
    g_lightInfo.color = glm::vec3(1.0f, 1.0f, 1.0f);

    g_materials[3].aColor = g_lightInfo.color;

    g_newx = g_oldx = g_width/2;
    g_newy = g_oldy = g_height/2;

    
    g_model_manager = new ModelManager();

}



// == initGeom ===
// initialize the geomtry (including color)
// Change file name to load a different mesh file

void initGeom() {

    phong = new PhongShader();

    initGround();

    LoadTexture("grass.bmp", bricksTex);
    // LoadTextureAlpha("smoke32.bmp", 1);
    // LoadTexture("bricks-norm.bmp", bricksNormTex);


    initfont(NULL);

    // initialize object database
    // this includes what objects are available to place in the map
    // when exporting, choose one.
    ExportObject entry;
    GameDrawableObject *model = new GameDrawableObject("cube");


    // object types
    // name is type here, really
    entry.name = "building";
    entry.model = model;
    g_object_palette.push_back(entry);

    entry.name = "ramp";
    entry.model = new GameDrawableObject("ramp");
    g_object_palette.push_back(entry);

    entry.name = "wings";
    entry.model = new GameDrawableObject("ramp");
    g_object_palette.push_back(entry);

    entry.name = "speed_upgrade";
    entry.model = new GameDrawableObject("ramp");
    g_object_palette.push_back(entry);

    entry.name = "speed_boost";
    entry.model = new GameDrawableObject("ramp");
    g_object_palette.push_back(entry);

    entry.name = "jetpack";
    entry.model = new GameDrawableObject("ramp");
    g_object_palette.push_back(entry);

    entry.name = "turning";
    entry.model = new GameDrawableObject("ramp");
    g_object_palette.push_back(entry);


    // dummy  model
    g_cube = model;

    toggleObjects(0);
}



void initGround() {

    // TODO - option to change ground pos?
    mat4 xl = translate(mat4(1.0f), vec3(0, 0, 0));
    // mat4 sc  = glm::scale(mat4(1.0f), vec3(g_groundSize, 0.1, g_groundSize));


    cout << "mat4 xl: \n";

    printMat4(xl, stdout);

    g_ground_xform = xl;

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
    glVertexAttribPointer(phong->getPosLocation(), 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(phong->getPosLocation());

    glBindBuffer(GL_ARRAY_BUFFER, GNBuffObj);
    glVertexAttribPointer(phong->getNormLocation(), 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(phong->getNormLocation());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);

    glBindVertexArray(0);


}


void drawGround() {
    // set ground texture
    /*
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bricksTex);
    phong->setTexture(0);
    */

    setMaterial(0);
    setModel(g_ground_xform);

    // bind buffers
    glBindBuffer(GL_ARRAY_BUFFER, GrndBuffObj);
    glVertexAttribPointer(phong->getPosLocation(), 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(phong->getPosLocation());

    glBindBuffer(GL_ARRAY_BUFFER, GNBuffObj);
    glVertexAttribPointer(phong->getNormLocation(), 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(phong->getNormLocation());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);

    // draw!
    glDrawElements(GL_TRIANGLES, g_GiboLen, GL_UNSIGNED_SHORT, 0);
}


// draw new one
void drawMesh() {

    GameDrawableObject *model = g_object_palette[g_current_object].model;

    // lame as fuck
    model->setPosition(g_export_trans);
    model->setScale(g_export_scale);
    model->setRotation(g_export_rot_vec3);
    model->draw(phong);

    // the old days
    // cube->render();
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

    sprintf(fpsstr, "Cam pos: x: %3.2f y: %3.2f z: %3.2f",
                     g_CamPos.x, g_CamPos.y, g_CamPos.z);
    render_text(fpsstr, -1 + 8 * sx, -0.8, sx, sy);


    // selected Pos
    sprintf(fpsstr, "selected pos: x: %3.2f y: %3.2f z: %3.2f",
                     g_export_trans.x, g_export_trans.y, g_export_trans.z);
    render_text(fpsstr, 0.2, 0.8, sx, sy);

    sprintf(fpsstr, "object type: %s",
            g_object_palette[g_current_object].name.c_str());
    render_text(fpsstr, 0.2, 0.7, sx, sy);
    

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


    // --- draw floating mesh
    setMaterial(g_current_material);

    drawMesh();


    // draw ground 
    drawGround();


    list<ExportObject>::iterator it;
    for (it=g_export_objects.begin(); it != g_export_objects.end(); it++) {

        setMaterial(it->materialIndex);

        it->model->setPosition(it->t);
        it->model->setScale(it->s);
        it->model->setRotation(it->r);

        it->model->draw(phong);
    }



    // --- draw light ---
    setMaterial(3);
    mat4 sc = glm::scale(mat4(1.0f), vec3(0.1f));
    mat4 xl = translate(mat4(1.0f), g_lightInfo.pos);
    // setModel(xl * sc);
    // TODO - draw white cube or model


    drawTexts();

    glutSwapBuffers();
}


float maxV = 2;
float currentV;

float a = 4;


void updateScene(float t, float dt) {

    // rockets->update(t, dt);
    // planes->update(t, dt);

    // bunnies->update(t, dt, g_CamPos, &g_score);


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

    // TODO - make option to rotate with respect to view coords

    switch (g_action) {
    case DO_XLATE:
        if (g_translate_direction == XT_XZ) {
            g_export_trans += glm::vec3(1.5*(x1), 0, - 1.5*(y1));
        } else {
            g_export_trans += glm::vec3(0, 1.5*(y1), 0);
        }
        break;
    case DO_SCALE:
        s = x1-x0 + y1-y0;
        g_scaleRotMat = glm::scale(g_scaleRotMat, glm::vec3(s + 1));
        g_export_scale *= s+1;

        // change height so object still touches ground
        // FIXME - needs object's y-dimention
        // y' = s * y
        g_export_trans.y *= s + 1;


        break;
    case DO_ROTATE:
        // TODO - option to rotate only against 1 axis

        g_export_rot_vec3.y += 50 * x1;

        // freeform
        /*
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

        */
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

    char fname[100];



    switch( key ) {
    case 'y':
        printf("transform in y direction\n");
        g_translate_direction = XT_Y;
        break;
    case 'x':
        printf("transform in xz direction\n");
        g_translate_direction = XT_XZ;
        break;
    case '.':
        toggleObjects(1);
        break;  

    case ',':
        toggleObjects(-1);
        break;  

    case 'o':
        printf("enter map name: \n");
        scanf("%s", fname);
        
        printf("using '%s'.\n", fname);

        exportMap(fname);
        
        break;

    case 'j':
        toggleMaterials(-1);
        break;

    case 'k':
        toggleMaterials(1);
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
        g_xform = (g_xform) % 2;
        
        if (g_xform == XT_VIEW) {
            glutSetCursor(GLUT_CURSOR_NONE);
        }

        cout << "Xform " << xform_names[g_xform] << endl;
        break;

    /* WASD keyes affect view/camera transform */
    case 'w':
        playerDir.z = -1;

        break;
    case 'e':
        if (g_xform == XT_OBJ) {
            g_action = DO_SCALE;
        }
        break;
    case 's':

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
            g_export_trans = glm::vec3(0);
            g_scaleRotMat = glm::mat4(1.0f);
        }
        break;
    case 'q': case 'Q' :
        exit( EXIT_SUCCESS );
        break;
    }
}


GameDrawableObject* getDrawableObjectByName(const char* name) {
    vector<ExportObject>::iterator it;
    for (it=g_object_palette.begin(); it != g_object_palette.end(); it++) {
        if (name == it->name) {
            return it->model;
        }
    }
    printf("Warning: loadMap: don't know object type '%s' using cube\n", name);

    return g_cube;
}


ExportObject getCurrentObject() {

    ExportObject o;

    // copy everything
    o = g_object_palette[g_current_object];

    o.materialIndex = g_current_material;
    o.t = g_export_trans;
    o.s = g_export_scale;
    o.r = g_export_rot_vec3;

    return o;
}


// == Export map
// saves g_export_objects to a file.
//
bool exportMap(const char *filename) {

    FILE * file = fopen(filename, "w+");

    if (NULL == file) {
        fprintf(stderr, "Couldn't open %s for exporting map\n", filename);
        return false;
    }

    fprintf(file, "%d\n", (int) g_export_objects.size());

    list<ExportObject>::iterator it;
    for (it=g_export_objects.begin(); it != g_export_objects.end(); it++) {
        fprintf(file, "%s %d\n", it->name.c_str(), it->materialIndex);
        // printMat4(it->xt, file);
        fprint3f(it->t, file);  
        fprint3f(it->s, file);  
        fprint3f(it->r, file); 
    }

    fclose(file);
    
    return true;
}


bool loadMap(const char* filename, list<ExportObject> &map) {

    FILE * file = fopen(filename, "r");

    if (NULL == file) {
        fprintf(stderr, "Couldn't open %s for reading the map\n", filename);
        return false;
    }

    int num_objs = 0;

    fscanf(file, "%d ", &num_objs); 

    list<ExportObject>::iterator it;

    
    ExportObject reconstructedObject;

    char name[100];
    int matnum;
    mat4 rotmat;

    while (num_objs--) {
        fscanf(file, "%s %d\n", name, &matnum);
        reconstructedObject.model = getDrawableObjectByName(name);

        read3f(reconstructedObject.t, file);
        printf("read t:\n");
        fprint3f(reconstructedObject.t, stdout);

        read3f(reconstructedObject.s, file);
        printf("read s:\n");
        fprint3f(reconstructedObject.s, stdout);

        read3f(reconstructedObject.r, file);
        printf("read r:\n");
        fprint3f(reconstructedObject.r, stdout);

        reconstructedObject.materialIndex = matnum;
        reconstructedObject.name = name;

        reconstructedObject.xt = scale(mat4(1.0), reconstructedObject.s) * 
                                 translate(mat4(1.0), reconstructedObject.t);

        map.push_back(reconstructedObject);
    }

    /* with mats
    while (num_objs--) {
        fscanf(file, "%s %d\n", name, &matnum);
        readMat4(reconstructedObject.xt, file);
        printf("read mat4\n");
        printMat4(reconstructedObject.xt, stdout);
        reconstructedObject.materialIndex = matnum;
        reconstructedObject.name = name;
        map.push_back(reconstructedObject);
    }
    */

    fclose(file);
    
    return true;

}


// ===== end Export Objects =====



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

                ExportObject o = getCurrentObject();
                // o.xt = translate(mat4(1.0f), g_export_trans) * g_scaleRotMat;
                cout << "Put " << o.name << endl;
                g_export_objects.push_back(o);
            }
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


    initGeom();

    std::string fname;
    if (argc > 1) {
        loadMap(argv[1], g_export_objects);
    }

    
    glutMainLoop();
    return 0;
}



