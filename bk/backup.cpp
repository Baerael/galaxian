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
#include "vec2d.h"

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
    }

    void set_x(int a)
    {
        x = a;
    }

    void set_y(int b)
    {
        y = b;
    }

    bool inbound()
    {
        return y >= 30 ? true : false;
    }
    

    void setLive()
    {
        isLive = true; 
    }
    
    void setL()
    {
        isLive = false; 
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
          laser_num(0), size(100)
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
        for (int i = 0; i < laser_num + 1; i++)
        {
            std::cout << 'i' << i << '\n';
            if (laser[i].get_isLive() && laser[i].inbound())
            {
                laser[i].draw(surface, rect);
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
            laser[laser_num].set_x(x + rect.w / 2 - 1);
            laser[laser_num].set_y(y);
            laser[laser_num].setLive();
            laser_num++;
        }
    }

    


//private:
    int x, y;
    int speed;
    Image image;
    Rect rect;
    Laser laser[1000];
    int laser_num;
    int size;
};


class Star
{
public:
    // must set default value or you wont have a defaut constructor
    Star(int x=0, int y=0, int speed=3, int r=255, int g=255, int b=255)
        : x_(x), y_(y), speed_(speed), r_(r), g_(g), b_(b)
    {
        speed_ = rand() % 2 + 1;
        if (speed_ == 1)
        {
            r_ = 150; g_ = 150; b_ = 150;
        }
        else
        {
            r_ = 150; g_ = 150; b_ = 250;
        }
    }
    void draw(Surface & surface) const
    {
        //surface.put_pixel(x_, y_, r_, g_, b_);
        surface.put_circle(x_, y_, 6, r_, g_, b_);
    }
    void run()
    {
        y_ += speed_;
        if (y_ > H - 1) 
        {
            x_ = rand() & W;
            y_ = 0;
        }

    void set_x(int x)
    {
        x_ = x;
    }
    void set_y(int y)
    {
        y_ = y;
    }
private:
    int x_, y_;
    vec2d p_;
    int speed_;
    int r_, g_, b_;
};



void game()
{
    srand((unsigned int) time(NULL));
    Surface surface(W, H);
    Event event;

    // put laser here
    Spaceship spaceship;
    //Star star(100, 100);
    Star star[10];
    for (int i = 0; i < 20; ++i)
    {
        star[i].set_x(rand() % W);
    }

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
        /// Move
        for (int i = 0; i < 20; ++i)
        {
            star[i].run();
        }


        // draw
        surface.lock();
        surface.fill(BLACK);
        spaceship.draw(surface);
        for (int i = 0; i < 20; ++i)
        {
            star[i].draw(surface);
        }
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
