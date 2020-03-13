#ifndef CORE_HPP
#define CORE_HPP

#ifdef WIN32
#define M_PI	3.1415926f
#include <windows.h>
#endif

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

/*
This file just has a bunch of common stuff used by all objects. It mainly just
includes GL and some other standard headers.
*/

void drawAxis(float size);

void drawWireBox(float xmin,float ymin,float zmin,float xmax,float ymax,float zmax);

#endif
