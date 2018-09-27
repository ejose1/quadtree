#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <sstream>

#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"
#include "gameobject.h"
#include "quadtree.h"

int rand_speed()
{

}

int main()
{
    Surface surface(W, H);
    Event event;
    Mouse mouse;
    
    
    srand((unsigned int) time(NULL));
        
    std::vector < GameObject * > r;

    std::vector< GameObject * > collision;

    
    bool spawn = false;
    int spawn_timer = 0;
    double theta;
    int size = 3000;
    int index = 0;
    int spawn_size = 0;
    int spawn_height = 100;
    bool normal_collision = true;

    while (1)
    {
        if (event.poll() && event.type() == QUIT) break;
        else if (event.type() == MOUSEMOTION)
        {
            mouse.update(event);
        }
        else if (event.type() == MOUSEBUTTONDOWN)
        {
            mouse.update(event);
            if(mouse.left())
            {
                spawn = true;
            }
            else if (mouse.right())
            {
                spawn = false;
            }
        }
        
        KeyPressed keypressed = get_keypressed();
        if (keypressed[SDLK_1])
        {
            normal_collision = false;
        }
        else if (keypressed[SDLK_2])
        {
            normal_collision = true;
        }

        if (spawn)
        {
            if (spawn_timer == 10)
            {
                GameObject * p = new GameObject(mouse.x(),  mouse.y(), 8, 8, 0,
                                                surface, rand() % 11 - 5,
                                                rand()% 11 - 5, W, H);
                r.push_back(p);
                ++spawn_size;
                spawn_timer = 0;
            }
            else
            {
                ++spawn_timer;
            }
        }
            
        if (index < size)
        {
            GameObject * p = new GameObject((W / size) * (index / 3) + index / 6,
                                            (H / 4),
                                            4, 4, 0,
                                            surface, rand() % 11 - 5,
                                            rand() % 11 - 5, W, H);
            GameObject * q = new GameObject((W / size) * (index / 3) + index / 6,
                                            (H / 2),
                                            4, 4, 0,
                                            surface, rand() % 11 - 5,
                                            rand() % 11 - 5, W, H);
            GameObject * o = new GameObject((W / size) * (index / 3) + index / 6,
                                            (3 * H / 4),
                                            4, 4, 0,
                                            surface, rand() % 11 - 5,
                                            rand() % 11 - 5, W, H);
            r.push_back(p);
            r.push_back(q);
            r.push_back(o);
            index += 3;
        }
        
        // update each object's position
        for (int i = 0; i < r.size(); ++i)
        {
            r[i]->run();
        }

        // if all objects are teh same color, change to next state
        int count = 0;
        for (int i = 1; i < r.size(); ++i)
        {
            if (r[0]->state() == r[i]->state())
            {
                ++count;
            }
        }

        if (count == size - 1 + spawn_size)
        {
            std::cout << "SPAWNING\n";
            r[rand() % r.size()]->update_state();
        }

        //----------------------------------------------------------------------
        // check collisions
        //----------------------------------------------------------------------

        std::vector< Line > l;

        // check every object against every other object
        if (normal_collision)
        {
            for (int i = 0; i < r.size(); ++i)
            {
                for (int j = i + 1; j < r.size(); ++j)
                {
                    if(check_collision(r[i], r[j]))
                    {
                        collision.push_back(r[i]);
                        collision.push_back(r[j]);
                    }
                }
            }
        }
        else
        {
            QTNode * proot = new QTNode();
            QuadTree qt(proot);

            for (int i = 0; i < r.size(); ++i)
            {
                qt.insert(qt.proot(), r[i]);
            }

            
            // preorder_qt_print(qt.proot());

            create_line(l, qt.proot());

            qt.who_collides(qt.proot(), collision);

            qt.clear();
        }

        //----------------------------------------------------------------------
        // collision resolution
        //----------------------------------------------------------------------
        for (int i = 0; i < collision.size(); i += 2)
        {
            // update the movement of the gameobjects
            collision[i]->collision();
            collision[i + 1]->collision();

            // if one of the game objects is red and the other green
            // make them both green
            if (collision[i]->state() == 1 && collision[i + 1]->state() == 0 ||
                collision[i]->state() == 0 && collision[i + 1]->state() == 1)
            {
                if (collision[i]->state() > collision[i + 1]->state())
                {
                    collision[i + 1]->update_state();
                }
                else
                {
                    collision[i]->update_state();
                }
            }
            // if one of the game objects is green and the other blue
            // make them both blue
            else if (collision[i]->state() == 2 &&
                     collision[i + 1]->state() == 1 ||
                     collision[i]->state() == 1 &&
                     collision[i + 1]->state() == 2)
            {
                if (collision[i]->state() > collision[i + 1]->state())
                {
                    collision[i + 1]->update_state();
                }
                else
                {
                    collision[i]->update_state();
                }
            }
            // if one of the game objects is blue and the other red
            // make them both red
            else  if (collision[i]->state() == 0 &&
                      collision[i + 1]->state() == 2 ||
                      collision[i]->state() == 2 &&
                      collision[i + 1]->state() == 0)
            {
                if (collision[i]->state() < collision[i + 1]->state())
                {
                    collision[i + 1]->update_state();
                }
                else
                {
                    collision[i]->update_state();
                }
            }
        }               

        // empty the collision vector
        collision.clear();
        
        // draw each object
        surface.lock();
        surface.fill(BLACK);
        for (int i = 0; i < r.size(); ++i)
        {
            r[i]->draw();
        }

        for (int i = 0; i < l.size(); ++i)
        {
            l[i].draw(surface);
        }

        surface.unlock();
        surface.flip();

        delay(10);
    }

    return 0;
}




