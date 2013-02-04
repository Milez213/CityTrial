#ifndef TTF_RENDERER_H
#define TTF_RENDERER_H

// This class implements True Type text rendering
//
// Created by Mustafa Khafateh, 4/2/2013
// 
// Only create one instance!
// 
// 
//

#include <stdio.h>

#include <ft2build.h>
// This is correct
#include FT_FREETYPE_H

#include <GL/gl.h>


#include "FontShader.h"
#include "TextRenderer.h"


/* TODO - this is better design. but don't wanna spend time on it now
 
class TTFRendererFactory {

    virtual void init() {

    }



};
*/


class TTFRenderer : public TextRenderer {

public:

    virtual ~TTFRenderer() {
         
    };

    TTFRenderer(const char* facename) {

        /*
        // Attempt at using static var
        if (id != 0) {
            fprintf(stderr, "***WARNING: creating another instance of \
                    TTFRendderer. Should only have 1 instance\n");
            return;
        }
        id++;
        */
        
        init(facename);


    }


    /**
     * from opengl programming on wikibooks.org
     * x and y are in -1, 1 cordinates
     * sx and sy are scale
     *
     * Render text using the currently loaded font and currently set font size.
     * Rendering starts at coordinates (x, y), z is always 0.
     * The pixel coordinates that the FreeType2 library uses are scaled by (sx, sy).
     *
     * TODO - use sy, sx    
     */
    virtual void drawText(const char *text, float x, float y,
                          float sx, float sy) {

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


        /* Enable blending, necessary for our alpha texture */
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        GLint attribute_coord = fontShader->getAtribCoord();

        // use shader
        fontShader->use();

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


protected:
    virtual void init(const char *fontfilename) {

        font_size = 36;

        /* Initialize the FreeType2 library */
        if (FT_Init_FreeType(&ft)) {
            fprintf(stderr, "ERROR: TTFRenderer: Could not init freetype library\n");
            return;
        }

        /* Load a font */
        if (FT_New_Face(ft, fontfilename, 0, &face)) {
            fprintf(stderr, "ERROR: TTFRenderer: Could not open font %s\n", fontfilename);
            return;
        }

        FT_Set_Pixel_Sizes(face, 0, font_size);

        glGenBuffers(1, &fontVBO);

        fontShader = new FontShader();

    }

protected:

    struct point {
        GLfloat x;
        GLfloat y;
        GLfloat s;
        GLfloat t;
    };

    int font_size;
    FT_Library ft;
    FT_Face face;

    
    // GLuint fontVAO; // TODO - use or discard
    GLuint fontVBO;

    FontShader *fontShader;

public:
    int id;

};

#endif
