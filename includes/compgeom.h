/*****************************************************************************
 Yihsiang Liow
 Copyright
*****************************************************************************/

/*****************************************************************************
File: compgeom.h

This file contains some basic (inline) computational geometric functions.
*****************************************************************************/
#ifndef COMPGEOM_H
#define COMPGEOM_H

//----------------------------------------------------------------------------
// Returns index i if point (qx, qy) is on hor/ver line segments (px[i],py[i]) 
// to (px[i+1],py[i+1]). The value of size is the length of the array.
// The value of step indicates the direction of search: For step = 1, the
// search for i starts at 0. When step = -1, the search starts at size - 2.
//----------------------------------------------------------------------------
inline
int onsegments(int qx, int qy, int px[], int py[], int size, int step = 1)
{
	int start = 0, end = size - 2;
	if (step == -1)
	{
		start = size - 1;
		end = 1;
	}

	int index = -1;
	if (step == 1)
	{
		for (int i = 0; i <= size - 2; i++)
		{
			if (((qx == px[i]) && (py[i] <= qy && qy <= py[i + 1] || py[i] >= qy && qy >= py[i + 1]) )
				|| ((qy == py[i]) && (px[i] <= qx && qx <= px[i + 1] || px[i] >= qx && qx >= px[i + 1])))
			{
				index = i;
				break;
			}
		}
	}
	else if (step == -1)
	{
		for (int i = size - 2; i >= 0; i--)
		{
			if (((qx == px[i]) && (py[i] <= qy && qy <= py[i + 1] || py[i] >= qy && qy >= py[i + 1]) )
				|| ((qy == py[i]) && (px[i] <= qx && qx <= px[i + 1] || px[i] >= qx && qx >= px[i + 1])))
			{
				index = i;
				break;
			}
		}
	}
	else
	{
		std::cout << "ERROR in onsegments() ... step:" << step << std::endl;
	}

	return index;
}

//----------------------------------------------------------------------------
// area(x, y, n) returns the area of the polygon enclosed by (x[i], y[i]) for
// i = 0, 1, ..., n-1. 
// To avoid overflow, integers are cast to doubles. The return value is a 
// double. 
// WARNING: The maximum n is 1000.
//----------------------------------------------------------------------------
inline
double area(int x[], int y[], int n)
{
	double a = 0.0;
	double _x[1000], _y[1000];
	for (int i = 0; i < n; i++)
	{
		_x[i] = x[i]; _y[i] = y[i];
	}
	std::cout << std::endl;
	for (int i = 0; i < n - 1; i++)
	{
		a += (_x[i] + _x[i+1]) * (_y[i+1] - _y[i]);
	}
	a /= 2;
	return a;
}

inline 
int max(int x, int y)
{
	return (x > y ? x : y);
}

inline
bool segments_intersect(int x0, int y0, int x1, int y1, int X0, int Y0, int X1, int Y1)
{
	// ax + by = c' (line passing through x0,y0,x1,y1)
	// Ax + By = C' (line passing through X0,Y0,X1,Y1)
	//
	// y = m x + c
	// m = dy / dx
	// c = y0 - m x0
	// dx y = dy x + dx c
	// -dy x + dx y = dx c = dx y0 - dy x0 
	//
	// a = -dy
	// b = dx
	// c' = dx y0 - dy x0
	// 
	// M = [a b]
	//     [A B]
	// detM = aB - Ab

	int dx = x1 - x0;
	int dy = y1 - y0;
	int a = -dy;
	int b = dx;
	int c = dx * y0 - dy * x0;

	int dX = X1 - X0;
	int dY = Y1 - Y0;
	int A = -dY;
	int B = dX;
	int C = dX * Y0 - dY * X0;

	int det = a * B - A * b;
	if (det == 0) 
	{
		return false;
	}
	else
	{
		// compute point of intersection (x,y)
		double invdet = 1.0 / det;
		double x = invdet * (B * c - b * C);
		double y = invdet * (-A * c + a * C);
		// return true if (x,y) is on line segments
		return (min(x0, x1) <= x && x <= max(x0, x1) && min(y0, y1) <= y && y <= max(y0, y1))
			&& (min(X0, X1) <= x && x <= max(X0, X1) && min(Y0, Y1) <= y && y <= max(Y0, Y1));
	}
}

// Let q = (qx, qy) and p be the polygon described by _px, _py
// inpoly() will return 
// 'i' if q is strictly in the interior of p
// 'o' if q is strictly in the exterior of p
// 'e' if q is on an edge but not an endpoint
// 'v' if q is on an endpoint
// O'Rourke p244
inline 
char inpoly(int qx, int qy, int _px[], int _py[], int size)
{
	int Rcross = 0;
	int Lcross = 0;
	bool Rstrad, Lstrad;

	int px[1000], py[1000];
	for (int i = 0; i < size; i++)
	{
		px[i] = _px[i] - qx;
		py[i] = _py[i] - qy;
	}

	for (int i = 0; i < size; i++)
	{
		if (px[i] == 0 && py[i] == 0) return 'v';
		int i1 = (i + size - 1) % size;

		Rstrad = (py[i] > 0) != (py[i1] > 0);
		Lstrad = (py[i] < 0) != (py[i1] < 0);

		if (Rstrad || Lstrad)
		{
			int x = (int) ((px[i] * py[i1] - px[i1] * py[i]) / (double)(py[i1] - py[i]));
			if (Rstrad && x > 0) Rcross++;
			if (Lstrad && x < 0) Lcross++;
		}
	}
	if ((Rcross % 2) != (Lcross % 2)) return 'e';
	if ((Rcross % 2) == 1) return 'i';
	else return 'o';
}

#endif
