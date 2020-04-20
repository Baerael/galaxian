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

#ifndef GAME_H
#define GAME_H

class Part
{
public:
    // must set default value or you wont have a defaut constructor
    Part(int x=0, int y=0)
        : p_(x, y), v_(0, 0),
          is_alive_(false)
    {
        rate = 10;
        r_ = 255;
        g_ = 0;
        b_ = 0;
    }
    void draw(Surface & surface) const
    {
        surface.put_circle(int(p_[0]), int(p_[1]), 2, r_, g_, b_);
    }
    void run()
    {
      r_ -= 30;
      if (!is_alive_)
      {
          x_ = rand() % rand() % 2 - rand() % 4;
          y_ = rand() % 10 + 3;
      }
      p_[0] += x_;
      p_[1] += y_;

     //if (p_[1] <= 0 || p_[1] >= start)
     if (p_[1] <= (start - 10) || p_[1] >= (start + 25)
         )
     {
          start -= rate;
          std::cout << "rest\n";
          p_[0] = 300; // x
          p_[1] = start; // y
          is_alive_ = false;
          std::cout << p_[1] << '\n';
          r_ = 255;

          if (start == 0) start = 300;

      }
    }
    void set_rate(int r)
    {
        rate = r;
    }

    void set_x(int x)
    {
        p_[0] = x;
    }
    void set_y(int y)
    {
        p_[1] = y;
        start = y;
    }

//private:
    int rate;
    bool is_alive_;
    int time;
    vec2d p_;
    vec2d v_;
    int speed_;
    int r_, g_, b_;
    int x_, y_;
    int start;
};


//=================================================================
//  Spaceship
//=================================================================
class Spaceship
{
public:
    Spaceship()
        : image("images/galaxian/GalaxianGalaxip.gif"), speed_(6)
    {
        rect = image.getRect();
        rect.x = W / 2 - rect.w / 2;
        rect.y = H - 1 - 40 - rect.h;
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
        surface.put_image(image, rect);
    }

    void moveleft()
    {
        rect.x -= speed_;
        if (rect.x < 0) rect.x = 0;
    }
    void moveright()
    {
        rect.x += speed_;
        if (rect.x >= W - rect.w) rect.x = W - rect.w;
    }
    int & x()
    {
        return rect.x;
    }
    int & y()
    {
        return rect.y;
    }

//private:
    int speed_;
    Image image;
    Rect rect;
};

//=================================================================
//  Laser
//=================================================================
class Laser
{
public:
    Laser(int x=0, int y =0)
        : x_(x), y_(y), is_alive_(false),
          t(0)
    {}
    void run()
    {
        if (t == 0)
            for (int i = 0; i < 30; i++)
            {
                L_[i].set_x(x_);
                L_[i].set_y(y_);
                L_[i].set_rate(2);
            }
        if (is_alive_)
        {
            t = 0;
             for (int i = 0; i < 30; i++)
             {
                 L_[i].run();
             }
           y_ += speed_; // make speed variable
            if (y_ <= -8 && id_ == 0)
            {
                is_alive_ = false;
            }
            if (y_ >= 440 && id_ == 1)
            {
                is_alive_ = false;
            }
        }
    }
    void draw(Surface & surface)
    {
        // if (is_alive), then draw a rect
        if (is_alive_)
        {
            surface.put_rect(x_, y_, 2, 8, 255, 0, 0);
            for (int i = 0; i < 30; i++)
            {
                std::cout << "fire\n";
                L_[i].draw(surface);
            }
        }
    }
    int & id()
    {
        return id_;
    }
    bool & is_alive()
    {
        return is_alive_;
    }
    int & speed()
    {
        return speed_;
    }
    int & x()
    {
        return x_;
    }
    int & y()
    {
        return y_;
    }

private:
    int x_, y_;
    int id_;
    int bound_;
    int speed_;
    bool is_alive_;
    Part L_[30];
    int t;
};


//=================================================================
//  Star
//=================================================================

class Star
{
public:
    // must set default value or you wont have a defaut constructor
    Star(int x=0, int y=0)
        : p_(x, y), v_(0, 0)
    {
        int case_ = rand() % 3 + 1;
        int color[4] = { 0, 60, 80, 180};
        double speed[4] = {0, 0.5, 1.8, 2.5};
        v_ = vec2d(0, speed[case_]);
       // v_ = vec2d(0, speed[case_]);
        r_ = rand() % 66 + color[case_];
        g_ = rand() % 66 + color[case_];
        b_ = rand() % 66 + color[case_];
    }
    void draw(Surface & surface) const
    {
        //surface.put_pixel(x_, y_, r_, g_, b_);
        surface.put_circle(int(p_[0]), int(p_[1]), 2, r_, g_, b_);
    }
    void run()
    {
        p_ += v_;
        if (p_[1] > H - 1) 
        {
            p_[0] = rand() % W;
            p_[1] = 0;
        }
    }

    void set_x(int x)
    {
        p_[0] = x;
    }
    void set_y(int y)
    {
        p_[1] = y;
    }
//private:
    vec2d p_;
    vec2d v_;
    int speed_;
    int r_, g_, b_;
};
//=================================================================
//  AlienBlue   DYNAMIC
//=================================================================
class DAlien
{
public:
    DAlien(const char filename[], int x, int y)
        : image(filename), speed_(2),
          is_alive_(true), state_(0), is_attack_(false),
          sound("sounds/ret.wav")
    {
        rect = image.getRect();
        rect.x = x; // W / 2 - rect.w /2;
        rect.y = y; //H - 1 - 40 - rect.h;
      //rect.x = rand() % W; // W / 2 - rect.w /2;
      //rect.y = rand() % 100 + 100; //H - 1 - 40 - rect.h;
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
        surface.put_circle(rect2.x, rect2.y, 2, 255, 255, 255);
        if (is_alive_)
        {
            surface.put_image(image, rect);
        }
    }

    bool & is_alive()
    {
        return is_alive_;
    }
    bool & is_attack()
    {
        return is_attack_;
    }
    int & x()
    {
        return rect.x;
    }
    int & y()
    {
        return rect.y;
    }
    void set_range(int r)
    {
        range = r;
        rect2.x = rect.x;
        rect2.y = rect.y;
    }

    //test trck
    void track()
    {
        rect2.x += speed_;
        if (rect2.x <= range)
        {
            speed_ = 2;
        }
        else if (rect2.x >= range - 360 + W)
        {
            speed_ = -2;
        }
    }
    void menu_move()
    {
        if (rect.x >= 140)
        {
            rect.x -= 4;
        }
    }

    void run()
    { 
        switch (state_)
        {
            case 0:
                rect.x += speed_;
                rect2.x += speed_;
                if (rand() % 5000 == 0 && is_alive_)
                {
                    state_ = 1;
                }
                else
                {
                    // stay in fleet -- move L, R
                    //rect.x += rand() % 6 - 3;
                    if (rect.x <= range)
                    {
                        speed_ = 2;
                    }
                    else if (rect.x >= range - 360 + W)
                    {
                        speed_ = -2;
                    }
                }
                break;
            case 1:
                //===== Attack =====
                track();
                rect.x += 2 * sin(rect.y / 50.0);
                rect.y += 2;
                is_attack_ = true;
                if (rect.y >= 420)
                {
                    is_attack_ = false;
                    rect.y = - 40;
                    state_ = 2;
                }
                break;
            case 2:
                // return to fleet
                track();
                if (rect.x < rect2.x &&
                    rect.y < rect2.y)
                {
                    rect.x += 2;
                    rect.y += 2;
                }
                else if (rect.x > rect2.x &&
                         rect.y < rect2.y)
                {
                    rect.x -= 2;
                    rect.y += 2;
                }
                else
                {
        //            sound.play();
                    rect.x = rect2.x;
                    rect.y = rect2.y;
                    state_ = 0;
                }
                break;
        }
    }

//private:
    int x_, y_;
    int range;
    int speed_;
    Image image; // should be static member
    Rect rect;  // this if for drawing
    Rect rect2;
    Sound sound;
    int state_;
    bool is_alive_;
    bool is_attack_;
};

class Fstring
{
public:
    Fstring()
        : size_(0), capacity_(1024)
    {}
    Fstring(const char s[])
        : capacity_(1024)
    {
        std::cout << "copy\n";
        int i = 0;
        while (s_[i] != '\0')
        {
            s_[i] = s[i];
            ++i;
        }
    }  

    void fdraw(Surface & surface)
    {
        int i = 0;
        while (s_[i] != '\0')
        {
            //image(font.render(use, White);
            std::cout << s_[i] << '\n';
        }
        index = i;
        std::cout << index << '\n';

        //surface.put_image(image, rect);
    }

//private:
    char s_[1024];
    int size_;
    int capacity_;
    int index;
 // Font font;
 // Image image;
 // Rect rect;
    
};


// Font class
class F
{
public:
    F(const char WORD[], const char FONT[],
      int FSIZE, int x, int y)

        : font(FONT, FSIZE), 
          image(font.render(WORD, WHITE))
    {
        rect = image.getRect();
        rect.x = x;
        rect.y = y;
    }
    void draw(Surface & surface)
    {
        surface.put_image(image, rect);
    }

  
    int & x()
    {
        return rect.x;
    }
    int & y()
    {
        return rect.y;
    }

//praivte:
    char use[50];
    int index;
    Font font;
    Image image;
    Rect rect;
    
};
 
class S
{
public:
    S(const char str[], const char FONT[], int FSIZE,
         int x, int y)
        : f(font.render(str, WHITE)), 
          font(FONT, 25)
    {
        rect = f.getRect();
        rect.x = x; // W / 2 - rect.w /2;
        rect.y = y; //H - 1 - 40 - rect.h;
    }
    void draw(Surface & surface)
    {
        surface.put_image(f, rect);
    }
    int & x()
    {
        return rect.x;
    }
    int & y()
    {
        return rect.y;
    }
//private:
    int x_, y_;
    Font font;
    Rect rect;
    Image f;
};

// Menu fonts
class Menu
{
public:
    Menu(const char filename[], const char f0[],
         const char f1[], const char FONT[], int FSIZE,
         int x, int y)
        : image(filename), f(font.render(f0, WHITE)), 
          font(FONT, 25)
    {
        rectr = f.getRect();
        rectr.x = x + 100; // W / 2 - rect.w /2;
        rectr.y = y; //H - 1 - 40 - rect.h;

        rect = image.getRect();
        rect.x = x; // W / 2 - rect.w /2;
        rect.y = y; //H - 1 - 40 - rect.h;

    }
    Image & get_image()
    {
        return image;
    }
    Rect & get_rect()
    {
        return rect;
    }

    void ck()
    {
        std::cout << "here" << std::endl;
        // number
        int n = 123406789;
        n *= 10; // adds 0 to the end to shift
        int inc = 10;
        int size = 0;
        char str[20];
        while (n / inc != 0)
        {
            inc *= 10;
            size++;
        }

        inc = 10;
        std::cout << "print" << std::endl;
        for (int i = size - 1; i >= 0; i--)
        {
            str[i] = n / inc % 10 + 48;
            inc *= 10;
        }
        for (int i = 0; i < 12; i++)
        {
            std::cout << int(str[i]) << std::endl;
        }
        str[size] = '\0';
        z = str;
        //f(font.render(z, WHITE));
    }

    void draw(Surface & surface)
    {
        surface.put_image(image, rect);
        surface.put_image(f, rectr);
    }
    int & x()
    {
        return rect.x;
    }
    int & y()
    {
        return rect.y;
    }

    void set_range(int r)
    {
        range = r;
        rect2.x = rect.x;
        rect2.y = rect.y;
    }
    void menu_move()
    {
        if (rect.x >= 140)
        {
            rect.x -= 4;
            rectr.x -= 4;
        }
    }

//private:
    char * z;
    int x_, y_;
    int range;
    int speed_;
    Font font;
    Image image; // should be static member
    Rect rect;
    Rect rectr;  // this if for drawing
    Rect rect2;
    Image f;
    int id_;
    char n0[4], n1[4], n2[4];
};

class Msg
{
public:
    Msg(const char f0[], const char FONT[], int FSIZE, Color & c,
         int x, int y, int set=0)
        : font(FONT,25), image(font.render(f0, c)),
          range(set)
    {
        rect = image.getRect();
        rect.x = x; // W / 2 - rect.w /2;
        rect.y = y; //H - 1 - 40 - rect.h;
        range = x;

    }
    Image & get_image()
    {
        return image;
    }
    Rect & get_rect()
    {
        return rect;
    }
    void move()
    {
        std::cout << range << std::endl;
        if (range <= rect.x)
            rect.x -= 10;
    }
    void draw(Surface & surface)
    {
        surface.put_image(image, rect);
    }
    int & x()
    {
        return rect.x;
    }
    int & y()
    {
        return rect.y;
    }

//private:

    char * z;
    int x_, y_;
    int range;
    int speed_;
    Font font;
    Color * c;
    Image image; // should be static member
    Rect rect;
};

//=================================
//  Explosion
//=================================
class Explosion
{
public:
    Explosion(char filename[], int x, int y)
        : image(filename), speed_(2)
    {
        rect = image.getRect();
        rect.x = x; // W / 2 - rect.w /2;
        rect.y = y; //H - 1 - 40 - rect.h;
        rect.w = 2;

    }
  //Image & set(char filename[])
  //{
  //    image(filename);
  //}
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
        std::cout << rect.w << '\n';
        surface.put_image(image, rect);
    }
    int & x()
    {
        return rect.x;
    }
    int & y()
    {
        return rect.y;
    }
    void set_range(int r)
    {
        range = r;
      //rect2.x = rect.x;
      //rect2.y = rect.y;
    }
//private:
    int x_, y_;
    int range;
    int speed_;
    Image image; // should be static member
    Rect rect;
};


class Particle
{
public:
    // must set default value or you wont have a defaut constructor
    Particle(int x=0, int y=0)
        : p_(x, y), v_(0, 0),
          range(400),
          is_alive_(false)
    {
        int case_ = rand() % 3 + 1;
        int color[4] = { 0, 60, 80, 180};
        double speed[4] = {0, 0.5, 1.8, 2.5};
        v_ = vec2d(0, speed[case_]);
       // v_ = vec2d(0, speed[case_]);
        r_ = rand() % 66 + color[case_];
        g_ = rand() % 66 + color[case_];
        b_ = rand() % 66 + color[case_];
    }
    void draw(Surface & surface) const
    {
        //surface.put_pixel(x_, y_, r_, g_, b_);
        surface.put_circle(int(p_[0]), int(p_[1]), 2, r_, g_, b_);
    }
    void run()
    {
      std::cout << p_ << std::endl;
   
      int width = rand() % 20 + 1;
      if (r_ <= 0)
          r_ = 255;
      r_ -= 4;
      if (!is_alive_) 
      {
          //std::cout << "address: " << p_  << '\n';
          int select = rand() % 2;
          //std::cout << select << '\n';
          switch (select)
          {
              case 0:
                  x_ = rand() % rand() % width - rand() % width;
                  y_ = rand() % 10;
             //     std::cout << x_ << ' ' << y_ << '\n';
                  break;
                case 1:
                  x_ = rand() % rand() % width - rand() % width;
                  y_ = rand() % rand() % 10 - 10;
                //x_ = rand() % 2 - 4;
                //y_ = rand() % rand() % 5 - rand() % 4;
              //    std::cout << x_ << ' ' << y_ << '\n';
                  break;

                case 2:
                  x_ = rand() % rand() % 5;
                  y_ = rand() % rand() % width - rand() % width;
                  break;

                case 3:
                  x_ = rand() % rand() % 5 - 5;
                  y_ = rand() % rand() % width - rand() % width;
                  break;
          }
      }
      p_[0] += x_;
      p_[1] += y_;

   //if ((p_[1] <= 100 || p_[1] >= range) ||
   //    (p_[0] <= 100 || p_[0] >= 400))
     if ((p_[1] <= 0 || p_[1] >= 420) ||
         (p_[0] <= 0 || p_[0] >= 600))
     {
         r_ = 255;
         g_ = 50;
         b_ = 255;
          p_[0] = rand() % 500 - 100; // x
          p_[1] = rand() % 200; // y
          is_alive_ = false;
          range += 2;
          if (range >= 420) range = 250;
      }

      
    }

    void set_x(int x)
    {
        p_[0] = x;
    }
    void set_y(int y)
    {
        p_[1] = y;
    }
//private:
    int range;
    bool is_alive_;
    int time;
    vec2d p_;
    vec2d v_;
    int speed_;
    int r_, g_, b_;
    int x_, y_;
};
#endif
 




