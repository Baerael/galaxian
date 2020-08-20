// File: main.cpp

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
#include "Game.h"
void convert(char str[], int n);

void build_fleet(DAlien ** p_)
{
    //= Blue Alien
    for (int i = 0; i < 10; i++)
    {
        p_[i] = new DAlien("images/galaxian/GalaxianAquaAlien.gif",
                            i * 36, 36 * 3);
        p_[i]->set_range(i * 36);

        p_[i + 10] = new DAlien("images/galaxian/GalaxianAquaAlien.gif",
                            i * 36, 36 * 4);
        p_[i + 10]->set_range(i * 36);

        p_[i + 20] = new DAlien("images/galaxian/GalaxianAquaAlien.gif",
                            i * 36, 36 * 5);
        p_[i + 20]->set_range(i * 36);
    }
    //= Purple Alien
    for (int i = 0; i < 8; i++)
    {
        p_[i + 30] = new DAlien("images/galaxian/GalaxianPurpleAlien.gif",
                            i * 36 + 36, 36 * 2);
        p_[i + 30]->set_range(i * 36 + 36);
    }
    for (int i = 0; i < 6; i++)
    {
        p_[i + 38] = new DAlien("images/galaxian/GalaxianRedAlien.gif",
                            i * 36 + 36 * 2, 36);
        p_[i + 38]->set_range(i * 36 + 36 * 2); // 2 is the off set
    }
    //= Flag Ship
    p_[44] = new DAlien("images/galaxian/GalaxianFlagship.gif",
                        1 * 36 + 36 * 2, 0);
    p_[44]->set_range(2 * 36 + 36);
        
    p_[45] = new DAlien("images/galaxian/GalaxianFlagship.gif",
                        4 * 36 + 36 * 2, 0);
    p_[45]->set_range(5 * 36 + 36);



}



void play(Surface & surface, Event & event)
{
    Sound sound("sounds/laser.wav");
    Sound explosion("sounds/expl.wav");
    Sound explosion2("sounds/explosion.wav");

    Sound intro("sounds/intro.wav");
    //Sound return("sounds/return.wav");
    // Spaceship
    Spaceship spaceship;
    
    //======================================
    //  Set up fleet
    //======================================
    DAlien * p_[48];
    build_fleet(p_);
    

    int const L_SIZE = 100;
    Laser laser[L_SIZE];
    Part particle[L_SIZE];

    //=================
    // Star system
    //=================
    Star star[20];
    for (int i = 0; i < 20; i++)
    {
        star[i].set_x(rand() % W);
        star[i].set_y(rand() % W);
    }

    //==========
    // cpu timer
    //==========
    int RATE = 1000/ 50;

    int time = 0;
    int hit = 0;

    // rect for score
    Rect rect2;
    int score = 1;
    char s[20] = "0";
    char * z;

    char FONT[] = "fonts/kk.ttf";
    int FSIZE = 30;
    Font font(FONT, FSIZE);

    S * m_;
    m_ = new S("0", FONT, FSIZE, 0, 0);

    int points = 1;
    intro.play();
    while (1)
    {
        if (hit == 46 && time > 5)
        {
            for (int i = 0; i < 48; i++)
                delete  p_[i];

            build_fleet(p_);
            hit = 0;
        }
     
        if (event.poll() && event.type() == QUIT) break;
       int start = getTicks();

        KeyPressed keypressed = get_keypressed();
        if (keypressed[LEFTARROW])
        {
            spaceship.moveleft();
        }
        else if (keypressed[RIGHTARROW]) 
        {
            spaceship.moveright();
        }
        //=========================================
        // fire laser
        //=========================================
        if (keypressed[SPACE] && time > 5)
        {
            for (int i = 0; i < L_SIZE; i++)
            {
                if (!laser[i].is_alive())
                {
                    time = 0;
                    laser[i].id() = 0;
                    laser[i].speed() = -8;
                    laser[i].is_alive() = true;
                    laser[i].x() = spaceship.x() + 15;
                    laser[i].y() = spaceship.y() - 32;
                    sound.play();
                    break;
                }
            }
        }

        //=========================================
        /// Move
        //=========================================
        for (int i = 0; i < 20; i++)
            star[i].run();
        for(int i = 0; i < L_SIZE; i++)
            laser[i].run();


        for (int i = 0; i < 46; i++)
        {
            p_[i]->run();
        }


        //=========================================
        // check collisions
        //=========================================
        for (int i = 0; i < L_SIZE; i++)
        {
            for (int j = 0; j < 46; j++)
            {
                if (laser[i].id() == 0 &&
                    laser[i].x() >= p_[j]->x() &&
                    laser[i].x() <= (p_[j]->x() + 30) &&
                    laser[i].y() <= p_[j]->y() &&
                    laser[i].is_alive() && p_[j]->is_alive())
                {
                    if (rand() % 2 == 0)
                    {
                        explosion.play();
                    }
                    else
                    {
                        explosion2.play();
                    }
                    laser[i].is_alive() = false;
                    p_[j]->is_alive() = false;
                    hit++;
                    std::cout << hit << std::endl;
                    // points
                    points++;
                    convert(s, points);
                    delete m_;
                    m_ = new S(s, FONT, FSIZE, 0, 0);
                    
                }
            }
        }

        // alien laser
        for (int i = 0; i < 46; i++)
        {
            if (p_[i]->is_attack() && rand() % 50 == 0 &&
                p_[i]->is_alive())
            {
                for (int j = 0; j < L_SIZE; j++)
                {
                    if (!laser[i].is_alive())
                    {
                        laser[i].id() = 1;
                        laser[i].speed() = 4;
                        laser[i].is_alive() = true;
                        laser[i].x() = p_[i]->x() + 15;
                        laser[i].y() = p_[i]->y() + 32;
                        break;
                    }
                }
            }
        }


        //=========================================
        // draw
        //=========================================
        surface.lock();
        surface.fill(BLACK);
        for (int i = 0; i < 46; i++)
        {
            p_[i]->draw(surface);
        }
        for (int i = 0; i < 20; ++i)
        {
            star[i].draw(surface);
        }

        for(int i = 0; i < L_SIZE; i++)
        {
            laser[i].draw(surface);
        }

        // render score
        m_->draw(surface);

        spaceship.draw(surface);
        surface.unlock();
        surface.flip();

       int end = getTicks();
       int dt = RATE - end + start;
       std::cout << dt << '\n';
       if (dt > 0) delay(dt);

        if(time <= 10) time++;
    }

}

/*
void convert(char str[], int n)
{
    //int track = 10;
    for (int i = n - 1; i >= 0; i--)
    {
        str[i] = n / inc % 10 + 48;
        inc *= 10;
    }
    str[size] = '\0';
}
*/


void convert(char str[], int n)
{
    //int n = 12345679;
    n *= 10;
    int inc = 10;
    int size = 0;
    //char str[20];
    while (n / inc != 0)
    {
        inc *= 10;
        size++;
    }
    inc = 10;
    for (int i = size - 1; i >= 0; i--)
    {
        str[i] = n / inc % 10 + 48;
        inc *= 10;
    }
    str[size] = '\0';
}


void menu()
{ 
    Surface surface(W, H);
    Event event;

    // Set Alien images
    char FLAG[] = "images/galaxian/GalaxianFlagship.gif";
    char RED[] = "images/galaxian/GalaxianRedAlien.gif";
    char PURPLE[] = "images/galaxian/GalaxianPurpleAlien.gif";
    char BLUE[] = "images/galaxian/GalaxianAquaAlien.gif";

    // Set Font
    char FONT[] = "fonts/kk.ttf";
    int FSIZE = 30;

    // some garbages
    Font font(FONT, FSIZE);
    Color c = {255, 0, 0};
    Color b = {82, 219, 255};
    Color w = {255, 255, 255};

    
    // Menu points
    Menu * m_[3];
    m_[0] = new Menu(FLAG,   "60", "200", FONT, FSIZE, W + 100, 260);
    m_[1] = new Menu(RED,    "50", "200", FONT, FSIZE, W + 150, 290);
    m_[2] = new Menu(PURPLE, "40", "200", FONT, FSIZE, W + 200, 320);
    m_[3] = new Menu(BLUE,   "30", "200", FONT, FSIZE, W + 250, 350);

    // words
    Msg * p_[3];
    p_[0] = new Msg("WE ARE THE GALAXIANS", FONT, FSIZE, c, 140, 80);
    p_[1] = new Msg("MISSION: DESTROY ALIENS", FONT, FSIZE, c, 120, 120);
    p_[2] = new Msg("- SCORE ADVANCE TABLE -", FONT, FSIZE, w, 120, 160);
    p_[3] = new Msg("CONVOY  CHARGER", FONT, FSIZE, b, 178, 200);

    int select = 0;
    Msg * pts[3];
    pts[0] = new Msg("800  pts", FONT, FSIZE, b, 350, 260);
    pts[1] = new Msg("300  pts", FONT, FSIZE, b, 350, 260);
    pts[2] = new Msg("200  pts", FONT, FSIZE, b, 350, 260);
    pts[3] = new Msg("150  pts", FONT, FSIZE, b, 350, 260);

    Msg * pts1[0], * pts2[0], * pts3[0];
    pts1[0] = new Msg("100  pts", FONT, FSIZE, b, W + 358, 290, 358);
    pts2[0] = new Msg("80  pts", FONT, FSIZE, b, W + 365, 320, 365);
    pts3[0] = new Msg("60  pts", FONT, FSIZE, b, W + 365, 350, 365);

    Star star[20];
    for (int i = 0; i < 20; i++)
    {
        star[i].set_x(rand() % W);
        star[i].set_y(rand() % W);
    }   

    int frame = 0;
    int FPS = 0;

    int tend;
    int RATE = 1000 / 60;
    bool change = false;
    while (1)
    {
       if (event.poll() && event.type() == QUIT) break;
       int start = getTicks();

        KeyPressed keypressed = get_keypressed();
        if (keypressed[SPACE])
        {
            // make destructor
            break;
        }
        // Move
        for (int i = 0; i < 20; i++)
            star[i].run();
        for (int i = 0; i < 4; i++)
        {
            m_[i]->menu_move();
        }
        pts1[0]->move();
        pts2[0]->move();
        pts3[0]->move();
        
        surface.lock();
        surface.fill(BLACK);
        // draw stars
        for (int i = 0; i < 20; ++i)
            star[i].draw(surface);
        // draw aliens
        for (int i = 0; i < 4; i++)
        {
            m_[i]->draw(surface);
            p_[i]->draw(surface);
        }

        if (frame >= 30)
        {
            pts[select]->draw(surface);
            pts1[0]->draw(surface);
            pts2[0]->draw(surface);
            pts3[0]->draw(surface);
        }

        surface.unlock();
        surface.flip();

       

       int end = getTicks();
       int dt = RATE - end + start;
       if (dt > 0) delay(dt);
       //std::cout << frame << '\n';
       tend = getTicks() - start;
       if (tend != 0)
       {
           FPS = 1000 / tend;
       }
       frame++;
       if (frame >= 60) 
       {
           frame = 0; 
           select++;
           if (select > 2) select = 0;
       }

    }
    

}

//====================
//  Particle system
//====================
void particle()
{ 
    Surface surface(W, H);
    Event event;


    int SIZE = 100;
    Particle part[SIZE];
    for (int i = 0; i < SIZE; i++)
    {
        part[i].set_x(100);
        part[i].set_y(420);
    //    part[i].set_rate(10);

    }   

    int RATE = 1000 / 60;
    int select = 0;
    while (1)
    {
       if (event.poll() && event.type() == QUIT) break;
       int start = getTicks();

       // Move
       for (int i = 0; i < SIZE; i++)
            part[i].run();

        surface.lock();
        // draw stars
        surface.fill(BLACK);
        for (int i = 0; i < SIZE; ++i)
            part[i].draw(surface);

       surface.unlock();
       surface.flip();

       int end = getTicks();
       int dt = RATE - end + start;
       if (dt > 0) delay(dt);
       select++;
       if (select >= 10) 
       {
       //  for (int i = 0; i < 10; i++)
       //       m_[i]->x() += 10;
           select = 0;
       }


    }
}







int main(int argc, char* argv[])
{   
    srand((unsigned int) time(NULL));
    Surface surface(W, H);
    Event event;
 
    //particle();
    //test();
    menu();
    play(surface, event);
    
    return 0;
}

















