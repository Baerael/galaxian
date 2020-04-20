/*****************************************************************************
 Yihsiang Liow
 Copyright
 *****************************************************************************/
#ifndef RECT_H
#define RECT_H

#include <iostream>

class Rect
{
public:
	Rect(int x0 = 0, int y0 = 0, int w0 = 0, int h0 = 0)
		: x(x0), y(y0), w(w0), h(h0)
	{}
    Rect( const SDL_Rect & r)
		: x(r.x), y(r.y), w(r.w), h(r.h)
	{}
	SDL_Rect getSDL_Rect()
	{
		SDL_Rect r = {x, y, w, h};
		return r;
	}
	int x, y, w, h;
};

inline std::ostream & operator<<(std::ostream & cout, const Rect & r)
{
	cout << "(" << r.x << "," << r.y << "," << r.w << "," << r.h << ")";
	return cout;
}
#endif
