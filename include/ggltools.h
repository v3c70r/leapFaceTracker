#ifndef GLTOOLS_H
#define GLTOOLS_H

#include <GL/glut.h>
#include <stdio.h>

void gltBeginRasterText(int w, int h);

void gltRasterText(double x, double y, const char *text, 
                   void *font = GLUT_BITMAP_8_BY_13);
                   
void gltEndRasterText();
#endif
