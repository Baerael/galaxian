/*****************************************************************************
 Yihsiang Liow
 Copyright
 *****************************************************************************/
#ifndef SINGLETONS_H
#define SINGLETONS_H

#include "Surface.h"

class SingletonSurface
{
public:
	SingletonSurface() {}

	static Surface* getInstance() 
	{
		if(s == NULL)
		{
			s = new Surface();
			return s;
		}
		else
		{
			return s;
		}
	}


private:
	static Surface *s; 
};


#endif 
