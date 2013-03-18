#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

// Abstract class providing a text rendering interface
// Created by Mustafa Khafateh, 4/2/2013

class TextRenderer {


public:

    virtual ~TextRenderer() { /* empty */ };


    // static void init() = 0;

    virtual void init(const char *fontname) = 0;
    
    virtual void drawText(const char *text, float x, float y,
                          float sx, float sy, glm::vec3 color) = 0;


};


#endif

