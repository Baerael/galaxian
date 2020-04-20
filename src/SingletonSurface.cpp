/*****************************************************************************
 Yihsiang Liow
 Copyright
 *****************************************************************************/
#include "SingletonSurface.h"
#include <cmath>
#include "SDL.h"

Surface *SingletonSurface::s = NULL;

inline int abs(int x) { return x>=0 ? x : -x ;}
