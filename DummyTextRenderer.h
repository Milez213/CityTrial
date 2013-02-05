#ifndef DUMMY_TEXT_RENDERER_H
#define DUMMY_TEXT_RENDERER_H

// dummy class in case not using a real implementation
// Created by Mustafa Khafateh, 4/2/2013

#include "TextRenderer.h"


class DummyTextRenderer : public TextRenderer {


public:

    DummyTextRenderer(const char *name) {
        printf("* Using DummyTextRenderer\n");    
    };

    virtual ~DummyTextRenderer() {
    
    };


    virtual void init(const char *fontname) { /* empty */};
    
    virtual void drawText(const char *text, float x, float y,
                          float sx, float sy) { /* empty */ };


};


#endif

