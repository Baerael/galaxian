/****************************************************************************
 Yihsiang Liow
 Copyright
 ****************************************************************************/
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <sstream>

#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"




int main(int argc, char* argv[])
{
	//freopen("CON", "w", stdout);
	//freopen("CON", "w", stderr);

	// Prints to console window
	std::cout << "hello world" << std::endl;

	//test_event();
	//test_pixel();
	///test_line();
	//test_circle();
	//test_unfilled_circle();
	//test_rect();
	test_image();
	//helloworld();			// Of course we must have a hello world right?
	//test_keyboard();
	//test_key_up_down(); // NEW 2013
	//test_sound();
	//fancyhelloworld();		// eye candy
	//test_polygon(false);
	//test_polygon(true);

    // mouse
    //test_mouse(); // NEW 2013
    //test_mouse_event(); // NEW 2013
    //test_drag_n_drop(); // NEW 2013

    // gamepad
    //test_gamepad(); // NEW 2013
    
	return 0;
}
