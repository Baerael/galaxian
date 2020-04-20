/*****************************************************************************
 Yihsiang Liow
 Copyright
 *****************************************************************************/
#ifndef EVENT_H
#define EVENT_H

/*****************************************************************************

  An Event object models an event. The useful methods are poll() and type().
  The most useful values returned by type() are
  * QUIT
  * KEYDOWN
  * KEYUP

  USAGE: The following is an example involving the Event and Keyboard classes.

	Event event;
	Keyboard kbd;
	bool quit = false;

	while (!quit)
	{
		if (event.poll())
		{
			if (event.type() == QUIT)
			{ 
				quit = true;
			}
			else if (event.type() == KEYDOWN || event.type() == KEYUP)
			{
				kbd.update(event);
			}
		}
		if (kbd.keypressed(LEFTARROW))
			std::cout << "left key down\n";
		if (kbd.keypressed(RIGHTARROW))
			std::cout << "right key down\n";

	}

*****************************************************************************/
const Uint32 QUIT        = SDL_QUIT;
const Uint32 KEYDOWN     = SDL_KEYDOWN;
const Uint32 KEYUP       = SDL_KEYUP;
const Uint32 LEFTARROW   = SDLK_LEFT;
const Uint32 RIGHTARROW  = SDLK_RIGHT;
const Uint32 UPARROW     = SDLK_UP;
const Uint32 DOWNARROW   = SDLK_DOWN;
const Uint32 SPACE       = SDLK_SPACE;
const Uint32 TAB		 = SDLK_TAB;
const Uint32 MOUSEMOTION = SDL_MOUSEMOTION;
const Uint32 MOUSEBUTTONUP = SDL_MOUSEBUTTONUP;
const Uint32 MOUSEBUTTONDOWN = SDL_MOUSEBUTTONDOWN;
const Uint32 JOYSTICKAXISMOTION = SDL_JOYAXISMOTION;
const Uint32 JOYSTICKBUTTONDOWN = SDL_JOYBUTTONDOWN;
const Uint32 JOYSTICKBUTTONUP = SDL_JOYBUTTONUP;
const Uint32 JOYSTICKHATMOTION = SDL_JOYHATMOTION;


class Event;
class Keyboard;

class Event
{
public:

	int poll() 
	{
		return SDL_PollEvent(&event);
	}

	Uint32 type()
	{
		return event.type;
	}

	Uint8 keypressed(int code)
	{
		kbd = SDL_GetKeyState(NULL);
		return kbd[ code ];
	}

	SDL_KeyboardEvent * get_key()
	{
		return &event.key;
	}

	SDL_MouseMotionEvent * motion()
	{
		return &event.motion;
	}

    SDL_MouseButtonEvent * button()
    {
        return &event.button;
    }
    

    SDL_Event * get_event()
    {
        return &event;
    }
	SDL_Event event;
private:
	Uint8 * kbd;
};


/*****************************************************************************

  A Keyboard object models a simple keyboard. The Keyboard object considers a 
  key pressed until the key is released. While a key is pressed, if another
  key is pressed, the second key is ignored. (Hence this does not work in the
  case when you need Shift-A. But it's enough for your game).

  USAGE:
    If there is a KEYDOWN or KEYUP event type, then the update() method of the
	Keyboard class should be called. To check if the left arrow key is pressed,
	called keyboard.pressed(LEFTARROW).
	See above.

*****************************************************************************/
class Keyboard
{
public:
	Keyboard(Event & event) : keydown(false) {}

	void update(Event & event)
	{
		SDL_KeyboardEvent * key = event.get_key();
		switch (key->type)
		{
			case KEYDOWN:
			{
				if (!keydown)
				{
					keydown = true;
					sym = key->keysym.sym;
				}
				break;
			}

			case KEYUP:
			{
				if (keydown && key->keysym.sym == sym)
				{
					keydown = false;
				}
				break;
			}
		}
	}

	int get_sym()
	{
		return sym;
	}

	bool keypressed(int sym0)
	{
		return keydown && (sym == sym0);
	}

private:
	bool keydown;
	int sym;
};

/*****************************************************************************

  A Mouse object models a mouse. Right now this support only the mouse motion,
  i.e., it returns the x and y value of the relative mouse cursor position. 

  If there is a MOUSEMOTION event type, then the update() method of the Mouse
  class should be called.
	* x() returns the relative x position of the mouse cursor
	* y() returns the relation y position of the mouse cursor

  USAGE:
    Event event;
	Mouse mouse;
	bool quit
	while (!quit)
	{
	    if (event.poll())
		{
			switch (event.type())
			{
				case QUIT: 
					quit = true;
					break;

				case MOUSEMOTION:
					mouse.update(event);
					printf("%d, %d\n", mouse.x(), mouse.y());
			}
		}
	}

*****************************************************************************/
inline
int mouse_x()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    return x;
}

inline
int mouse_y()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    return y;
}

inline
bool mouse_left()
{
    return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1);
}

inline
bool mouse_middle()
{
    return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(2);   
}

inline
bool mouse_right()
{
    return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(3);   
}


class Mouse
{
public:
	void update(Event & event)
	{
        if (event.type() == MOUSEMOTION)
        {
            x0 = event.motion()->x;
            y0 = event.motion()->y;
            _leftbutton = event.motion()->state & SDL_BUTTON(SDL_BUTTON_LEFT);
            _middlebutton = event.motion()->state & SDL_BUTTON(SDL_BUTTON_MIDDLE);
            _rightbutton = event.motion()->state & SDL_BUTTON(SDL_BUTTON_RIGHT);
        }
        else if (event.type() == MOUSEBUTTONUP
                 || event.type() == MOUSEBUTTONDOWN)
        {
            x0 = event.button()->x;
            y0 = event.button()->y;
            _leftbutton = event.button()->button == SDL_BUTTON_LEFT;
            _middlebutton = event.button()->button == SDL_BUTTON_MIDDLE;
            _rightbutton = event.button()->button == SDL_BUTTON_RIGHT;            
        }
	};

	int x()
	{
		return x0;
	}

	int y()
	{
		return y0;
	}

    bool left()
    {
        return _leftbutton;
    }
    bool right()
    {
        return _rightbutton;
    }
    bool middle()
    {
        return _middlebutton;
    }

private:
	int x0, y0;
    bool _leftbutton, _rightbutton, _middlebutton;
};




inline
int num_joysticks()
{
    return SDL_NumJoysticks(); 
}


// Right now only one joystick because the event data is put into
// joystick 0.
class Joystick
{
public:
    Joystick(int id=0)
        : js(SDL_JoystickOpen(id)), index(id)
    {
        SDL_JoystickEventState(SDL_ENABLE);
    }

    ~Joystick()
    {
        SDL_JoystickClose(js);
    }
    
    int num_axes()
    {
        return SDL_JoystickNumAxes(js);
    }

    int num_buttons()
    {
        return SDL_JoystickNumButtons(js);
    }

    void update(Event & e)
    {
        _axis = e.get_event()->jaxis.axis;
        _axis_value = e.get_event()->jaxis.value;
        _button = int(e.get_event()->jbutton.button);
        _hat_up = e.get_event()->jhat.value == SDL_HAT_UP;
        _hat_down = e.get_event()->jhat.value == SDL_HAT_DOWN;
        _hat_left = e.get_event()->jhat.value == SDL_HAT_LEFT;
        _hat_right = e.get_event()->jhat.value == SDL_HAT_RIGHT;
    }

    int axis()
    {
        return _axis;
    }

    int axis_value()
    {
        return _axis_value;
    }

    int button()
    {
        return _button;
    }

    bool hat_up()
    {
        return _hat_up;
    }
    
    bool hat_down()
    {
        return _hat_down;
    }
    
    bool hat_left()
    {
        return _hat_left;
    }
    
    bool hat_right()
    {
        return _hat_right;
    }
        
private:
    SDL_Joystick * js;
    int index;
    int _axis;
    int _axis_value;
    int _button;
    bool _hat_up, _hat_down, _hat_left, _hat_right;
};

#endif
