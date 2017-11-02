//
//  grafica.h
//  TazBot
//
//  Copyright (c) 2014 TazBot. All rights reserved.
//

#ifndef TazBot_grafica_h
#define TazBot_grafica_h

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include "gen_alg.h"
#include "lodepng.h"

//user interaction
extern bool paused;//=false;
extern bool draw;//=true;
//for drawing
extern char buf2[10];
extern int skipdraw;//=1;
extern bool drawFood;//=true;
extern int CLOSED;//=0; //if environment is closed, then no random bots are added per time interval
extern const int WWIDTH;// 1200 //dimensioni finestra
extern const int WHEIGHT;// 900
extern char buf[100];

void processNormalKeys(unsigned char key, int x, int y);
void processMouse(int button, int state, int x, int y);
void processMouseActiveMotion(int x, int y);
void changeSize(int w, int h);
void RenderString(float x, float y, void *font, const char* string, float r, float g, float b);
void drawCircle(float x, float y, float r);
void drawDiavolo(float x, float y, float r);
void renderScene();
void handleIdle();

#endif
