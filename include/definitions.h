#ifndef OBJ_LIB
#define OBJ_LIB

//#define VERBOSE_INCLUDE

#ifdef VERBOSE_INCLUDE
#pragma message("definitions.h")
#endif

#include <stdio.h>
#include <cstring>
#include <iostream>

#include <cstddef>
#include <math.h>
#include <vector>
#include <float.h>
#include <algorithm>

using namespace std;

typedef vector<double> GlyphPoly;
typedef vector<GlyphPoly*> GlyphOutline;
#define PIXEL_SCALE 32

#endif
