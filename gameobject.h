#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Surface.h"
#include <iostream>

class GameObject
{
public:
    GameObject(int x, int y, int w, int h, int state,
               Surface & surface, int dx, int dy,
               int W, int H)
        :x_(x), y_(y), w_(w), h_(h),
         state_(state),
         surface_(surface),
        dx_(dx), dy_(dy),
        W_(W), H_(H)
    {
        switch(state_)
        {
            case 0:
                r_ = 255; g_ = 0; b_ = 0;
                break;
            case 1:
                r_ = 0; g_ = 255; b_ = 0;
                break;
            case 2:
                r_ = 0; g_ = 0; b_ = 255;
                break;
        }
    }

    void draw()
    {
        surface_.put_rect(x_, y_, w_, h_, r_, g_, b_);
    }
            
    void run()
    {
        x_ += dx_;
        y_ += dy_;
                
        if (x_  >=  0 && x_ + w_ <= W_)
        {
            dx_ *= 1;
        }
        else
        {
            if (x_ < 0)
            {
                x_ = 0;
            }
            else
            {
                x_ = W_ - w_;
            }
            
            dx_ *= -1;
        }
                
        if (y_  >= 0 && y_ + h_ <= H_)
        {
            dy_ *= 1;
        }
        else
        {
            if (y_ < 0)
            {
                y_ = 0;
            }
            else
            {
                y_ = H_ - h_;
            }
            
            dy_ *= -1;
        }
    }

    void collision()
    {
        dx_ *= -1;
        dy_ *= -1;
    }

    int dx() { return dx_; }
    int dy() { return dy_; }
    int x() { return x_; }
    int y() { return y_; }
    int w() { return w_; }
    int h() { return h_; }
    
    int state() { return state_; }
    void state(int state) { state_ = state; }

    void update_state()
    {
        switch(state_)
        {
            case 0:
                r_ = 0; g_ = 255; b_ = 0;
                state_ = 1;
                break;
            case 1:
                r_ = 0; g_ = 0; b_ = 255;
                state_ = 2;
                break;
            case 2:
                r_ = 255; g_ = 0; b_ = 0;
                state_ = 0;
                break;
        }
    }
    
private:
    int x_, y_, w_, h_;
    int r_, g_, b_;
    int dx_, dy_;
    Surface & surface_;
    int state_;
    int W_, H_;
};


class Line
{
public:
    Line(int x0, int y0, int x1, int y1, int r, int g, int b)
        :x0_(x0), y0_(y0), x1_(x1), y1_(y1), r_(r), g_(g), b_(b)
    {}

    void draw(Surface & surface)
    {
        surface.put_line(x0_, y0_, x1_, y1_, r_, g_, b_);
    }

    int x0() { return x0_; }
    int x1() { return x1_; }
    int y0() { return y0_; }
    int y1() { return y1_; }
    
private:
    int x0_, y0_, x1_, y1_;
    int r_, g_, b_;
};

#endif
