// system includes
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <sstream>

// header includes
#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"

// put array of lasers in laser class

class Laser
{
public:
    Laser()
        : r(255), g(255), b(255), speed(2), isLive(false)
    {}

    void draw(Surface & surface, Rect & rect)
    {
        surface.put_rect(x, y, 2, 10, 255, 255, 255);
        y -= speed;
        isLive = y >= 30 ? true : false;
    }

    void set_x(int a)
    {
        x = a;
    }

    void set_y(int b)
    {
        y = b;
    }

    bool bound()
    {
    }

    void setLive()
    {
        isLive = true; 
    }

    bool get_isLive()
    {
        return isLive;
    }

//private:
    int r, g, b;
    int x, y;
    int w, h;
    int speed;
    bool isLive;
    //Rect rect
};


class Spaceship
{
public:
    Spaceship()
        : image("images/galaxian/GalaxianGalaxip.gif"), speed(5),
          laser_num(0), size(0)
    {
        rect = image.getRect();
        rect.x = W / 2 - rect.w / 2;
        rect.y = H - 1 - 40 - rect.h;
        x = rect.x;
        y = rect.y;
    }
    Image & get_image()
    {
        return image;
    }
    Rect & get_rect()
    {
        return rect;
    }

    void draw(Surface & surface)
    {
        for (int i = 0; i < laser_num; i++)
        {
            if (laser[i].get_isLive())
            {
                laser[i].draw(surface, rect);
                if (laser[i].get_isLive()) 
                { 
                    laser_num--; 
                }
            }
        }
        surface.put_image(image, rect);
    }

    void moveleft()
    {
        x -= speed;
        rect.x -= speed;
        if (x < 0) x = 0;
            std::cout << "1 " << x << ' ' << rect.w << '\n';
        if (rect.x < 0) rect.x = 0;
            std::cout << "2 " << rect.x << ' ' << rect.w << '\n';
    }

    void moveright()
    {
        x += speed;
        rect.x += speed;
        if (x >= W - rect.w) x = W - rect.w;
            std::cout << "1 " << x << ' ' << rect.w << '\n';
        if (rect.x >= W - rect.w) rect.x = W - rect.w;
            std::cout << "2 " << rect.x << ' ' << rect.w << '\n';
    }

    void fire_laser()
    {
        if (laser_num < 100)
        {
            laser_num++;
            std::cout << "fire laser:" << laser_num << '\n';
            laser[laser_num].set_x(x + rect.w / 2 - 1);
            laser[laser_num].set_y(y);
            laser[laser_num].setLive();
        }
    }

    


//private:
    int x, y;
    int speed;
    Image image;
    Rect rect;
    Laser laser[100];
    int laser_num;
    int size;
};



void game()
{
    Surface surface(W, H);
    Event event;

    Spaceship spaceship;

    while (1)
    {

        if (event.poll() && event.type() == QUIT) break;

        KeyPressed keypressed = get_keypressed();
        if (keypressed[LEFTARROW])
        {
            spaceship.moveleft();
        }
        else if (keypressed[RIGHTARROW]) 
        {
            spaceship.moveright();
        }

        if (keypressed[SPACE])
        {
            spaceship.fire_laser();
        }


        surface.lock();
        surface.fill(BLACK);
        spaceship.draw(surface);
        surface.unlock();
        surface.flip();

        delay(22);
    }
}




int main(int argc, char* argv[])
{
    game();


    
	return 0;
}
