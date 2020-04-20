/*****************************************************************************
 Yihsiang Liow
 Copyright
 ****************************************************************************/
#include <cmath>
#include "SDL.h"
#include "Surface.h"
#include "TextSurface.h"

inline int abs(int x) { return x>=0 ? x : -x ;}
//inline int min( int a, int b)	{ return ( a<b ? a : b); }


//----------------------------------------------------------------------------
// put_pixel functions for different bpp
//----------------------------------------------------------------------------
void put_pixel_bpp1( SDL_Surface * surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*p = pixel;
}

void put_pixel_bpp2( SDL_Surface* surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    *(Uint16 *)p = pixel;
}

void put_pixel_bpp3( SDL_Surface * surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN) 
	{
		p[0] = (pixel >> 16) & 0xff;
		p[1] = (pixel >> 8) & 0xff;
        p[2] = pixel & 0xff;
	} 
	else
	{
		p[0] = pixel & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = (pixel >> 16) & 0xff;
    }
}

void put_pixel_bpp4( SDL_Surface * surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    *(Uint32 *)p = pixel;
}


Surface::Surface( int w, int h, int bpp, int flags)
	: _w(w), _h(h)
{
    // ADDED JOYSTICK
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE | SDL_INIT_JOYSTICK);
	SDL_WM_SetCaption("C++!!!", NULL);
	surface = SDL_SetVideoMode(w, h, bpp, flags);
	
	if (surface == NULL) 
	{
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
		fflush(stdout);
        //exit(1);
    }
	atexit(SDL_Quit);

	bpp = surface->format->BytesPerPixel;

	switch (bpp)
	{
		case 1: put_pixel_fptr = &put_pixel_bpp1; break;
		case 2: put_pixel_fptr = &put_pixel_bpp2; break;
		case 3: put_pixel_fptr = &put_pixel_bpp3; break;
		case 4: put_pixel_fptr = &put_pixel_bpp4; break;
	}
}


void Surface::free()
{
	SDL_FreeSurface(surface);
}

Uint8 * Surface::get_pixel_addr(int x, int y)
{
	return (Uint8*)(Uint8*)surface->pixels + y * surface->pitch + x * bpp;
}

// Must call lock first
Uint32 Surface::get_pixel(int x, int y)
{
    int bpp = get_bpp();
    Uint8 * p = get_pixel_addr(x,y);

    switch (bpp) 
	{
		case 1: return *p;
		case 2: return *(Uint16 *)p;
		case 3: if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			        return p[0] << 16 | p[1] << 8 | p[2];
				else
					return p[0] | p[1] << 8 | p[2] << 16;
		case 4: return *(Uint32 *)p;
	    default:return 0;
    }
}


//----------------------------------------------------------------------------
// Must call lock first
//----------------------------------------------------------------------------
void Surface::put_pixel(int x, int y, const Color & color)
{
	put_pixel(x, y, map_rgb(color));
}

void Surface::put_pixel(int x, int y, int r, int g, int b)
{
	Color c; // = {r, g, b};
	c.r = r;
	c.g = g;
	c.b = b;

	put_pixel(x, y, c);
}

void Surface::put_pixel(int x, int y, Uint32 pixel)
{
	put_pixel_fptr(surface, x, y, pixel);
}

void Surface::draw_pixel(int x, int y, const Color & color)
{
	this->lock();
	put_pixel(x, y, map_rgb(color));
	this->unlock();
	this->update_rect(x, y, 1, 1);
}


//----------------------------------------------------------------------------
// Rectangle functions
//----------------------------------------------------------------------------
void Surface::put_rect(int x, int y, int w, int h, const Color & color)
{
	SDL_Rect rect = {x,y,w,h};
	Uint32 pixel = map_rgb(color);
	SDL_FillRect(surface, &rect, pixel);
}
//----------------------------------------------------------------------------
// Circle functions
//----------------------------------------------------------------------------
void Surface::put_circle(int x, int y, int r, Uint32 pixel)
{
	// adjusted for parts of circle outside surface

	int minx0 = (x - r >= 0 ? -r : -x);
	int maxx0 = (x + r <= _w - 1 ? r : _w - 1 - x);

	for (int x0 = minx0; x0 <= maxx0; ++x0)
	{
		int root = (int) sqrt((double)(r*r - x0*x0));

		int miny0 = (y - root >= 0 ? -root : -y);
		int maxy0 = (y + root <= _h - 1 ? root : _h - 1 - y);

		for (int y0 = miny0; y0 <= maxy0; ++y0)
		{
			put_pixel(x+x0, y+y0, pixel);
		}
	}
}


void Surface::put_circle(int x, int y, int r, const Color & color)
{
	put_circle(x, y, r, map_rgb(color));
}


void Surface::put_circle(int x, int y, int r, int R, int G, int B)
{
	SDL_Color color = {R,G,B};
	put_circle(x, y, r, color);
}

void Surface::draw_circle(int x, int y, int r, Uint32 pixel)
{
	lock();
	put_circle(x, y, r, pixel);
	unlock();
	flip();
}

void Surface::draw_circle(int x, int y, int r, int R, int G, int B)
{
	SDL_Color color = {R,G,B};
	draw_circle(x, y, r, map_rgb(color));
}

void Surface::put_unfilled_circle(int x, int y, int r, Uint32 pixel)
{
    // use SDL_gfx
    circleColor(surface, x, y, r, pixel);
}

void Surface::put_unfilled_circle(int x, int y, int r, const Color & color)
{
	put_unfilled_circle(x, y, r, map_rgb(color));
}

void Surface::put_unfilled_circle(int x, int y, int r, int R, int G, int B)
{
	SDL_Color color = {R,G,B};
	put_unfilled_circle(x, y, r, color);
}

//----------------------------------------------------------------------------
// Line functions
//----------------------------------------------------------------------------
void Surface::put_line(int x0, int y0, int x1, int y1, Uint32 pixel)
{
	int minx = min(x0,x1);
	int miny = min(y0,y1);
	int dy = y1-y0;
	int dx = x1-x0;

	if (dx == 0 && dy == 0)
	{
		put_pixel(x0, y0, pixel);
		return;
	}

	if (abs(dx)>= abs(dy))
	{
		double m = ((double)(y1-y0))/(x1-x0);
		double c = y0 - m*x0;
		double y = m*minx + c;
		for (int x=minx; x<=minx+abs(dx); x++)
		{
			put_pixel((int)x, (int)y, pixel);
			y += m;
		}
	}
	else
	{
		double n= ((double)(x1-x0))/(y1-y0);
		double c = x0 - n*y0;
		double x = n*miny + c;
		for (int y=miny; y<=miny+abs(dy); y++)
		{
			put_pixel((int)x, (int)y, pixel);
			x += n;
		}
	}
}

void Surface::put_line(int x0, int y0, int x1, int y1, const Color & c)
{
	put_line(x0, y0, x1, y1, map_rgb(c));
}

void Surface::put_line(int x0, int y0, int x1, int y1, int r, int g, int b)
{
	Color c = {r, g, b};
	put_line(x0, y0, x1, y1, map_rgb(c));
}

void Surface::draw_line(int x0, int y0, int x1, int y1, const Color & c)
{
	lock();
	put_line(x0, y0, x1, y1, c);
	unlock();
	flip();
}

//----------------------------------------------------------------------------
// flip()
// Flips the buffers
//----------------------------------------------------------------------------
void Surface::flip()
{
	SDL_Flip(surface);
}


int Surface::get_bpp()
{
	return surface->format->BytesPerPixel;
}


void Surface::update_rect(int x, int y, int w, int h)
{
	SDL_UpdateRect (surface, x, y, w, h);
}


Uint32 Surface::map_rgb(const SDL_Color & color)
{
	return SDL_MapRGB(surface->format, color.r, color.g, color.b);
}


//----------------------------------------------------------------------------
// Text drawing
//----------------------------------------------------------------------------
void Surface::put_text(TextSurface & text, int x, int y)
{
	// Source rectangle
	SDL_Rect source_rect;
	source_rect.x = source_rect.y = 0;
	source_rect.w = text.get_surface()->w;
	source_rect.h = text.get_surface()->h;

	// Destination rectangle
	SDL_Rect dest_rect;
	dest_rect = source_rect;
	dest_rect.x = x;
	dest_rect.y = y;

	SDL_BlitSurface(text.get_surface(), &source_rect, surface, &dest_rect);
}

void Surface::put_text(char str[], int x, int y,int R, int G, int B, char fontfamily[], int size)
{
	TextSurface text(str, fontfamily, size, R, G, B);
	put_text(text, x, y);
}

//------------------------------------------------------------------------
// Image drawing
//------------------------------------------------------------------------
void Surface::put_image(SDL_Surface *image, SDL_Rect *srcRect, SDL_Rect *dstRect)
{
	SDL_BlitSurface(image, srcRect, surface, dstRect);
}



//----------------------------------------------------------------------------
// lock()
// Locks the surface
//----------------------------------------------------------------------------
void Surface::lock()
{
	if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);
}


//----------------------------------------------------------------------------
// unlock()
// Unlocks the surface
//----------------------------------------------------------------------------
void Surface::unlock()
{
	if (SDL_MUSTLOCK(surface))
		SDL_UnlockSurface(surface);
}
