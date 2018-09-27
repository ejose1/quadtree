#include <iostream>
#include <vector>
#include "quadtree.h"
#include "gameobject.h"
#include "Surface.h"

int QTNode::threshold_ = 4;

void preorder_qt_print(QTNode * proot)
{
    if (proot == NULL)
    {
        // base case: do nothing
        return;
    }
    else
    {
        for (int i = 0; i < proot->depth_; ++i)
        {
            std::cout << '\t';
        }
        std::cout << (*proot) << std::endl;

        if (proot->children_[0] != NULL)
        {
            preorder_qt_print(proot->children_[0]);
            preorder_qt_print(proot->children_[1]);
            preorder_qt_print(proot->children_[2]);
            preorder_qt_print(proot->children_[3]);
        }
    }
}

bool check_collision(GameObject * good_guy, GameObject *  bad_guy)
{
    bool ret = false;

    int x0 = good_guy->x();
    int x1 = good_guy->x() + good_guy->w();
    int y0 = good_guy->y();
    int y1 = good_guy->y() + good_guy->h();
    
    int x0p = bad_guy->x();
    int x1p = bad_guy->x() + bad_guy->w();
    int y0p = bad_guy->y();
    int y1p = bad_guy->y() + bad_guy->h();
    
    if ((x0p >= x0 && x0p <= x1) || (x1p >= x0 && x1p <= x1))
    {
        if((y0p >= y0 && y0p <= y1) || (y1p >= y0 && y1p <= y1))
        {
            ret = true;
        }
    }
    
    return ret;
}

void QuadTree::who_collides(QTNode * proot,
                               std::vector< GameObject * > &collision_v)
{
    if (proot == NULL)
    {
        //do nothing
        return;
    }
    else
    {
        if (proot->parent_ != NULL)
        {
            // check any data left in the parent against child
            for (int i = 0; i < proot->parent_->data_.size(); ++i)
            {
                for (int j = 0; j < proot->data_.size(); ++j)
                {
                    if (check_collision(proot->parent_->data_[i],
                                        proot->data_[j]))
                    {
                        collision_v.push_back(proot->parent_->data_[i]);
                        collision_v.push_back(proot->data_[j]);
                    }
                }
            }
        }
        // check data in child against itself
        for (int i = 0; i < proot->data_.size(); ++i)
        {
            for (int j = i + 1; j < proot->data_.size(); ++j)
            {
                if (check_collision(proot->data_[i], proot->data_[j]))
                {
                    collision_v.push_back(proot->data_[i]);
                    collision_v.push_back(proot->data_[j]);
                }
            }
        }
        who_collides(proot->children_[0], collision_v);
        who_collides(proot->children_[1], collision_v);
        who_collides(proot->children_[2], collision_v);
        who_collides(proot->children_[3], collision_v);
    }
}

void create_line(std::vector< Line > & l, QTNode * proot)
{
    if (proot == NULL)
    {
        //do nothing
        return;
    }  
    else
    {
        if (proot->children_[0] != NULL)
        {
            l.push_back(Line((proot->minx_ + proot->maxx_) / 2,
                             proot->miny_,
                             (proot->minx_ + proot->maxx_) / 2,
                             proot->maxy_ - 1,
                             255, 0, 0));
            l.push_back(Line(proot->minx_,
                             (proot->miny_ + proot->maxy_) / 2,
                             proot->maxx_ - 1,
                             (proot->miny_ + proot->maxy_) / 2,
                             255, 0, 0));
            
            create_line(l, proot->children_[0]);
            create_line(l, proot->children_[1]);
            create_line(l, proot->children_[2]);
            create_line(l, proot->children_[3]);
        }
    }
}

void QuadTree::make_children(QTNode * proot)
{
    int depth = proot->depth_ + 1;

    proot->children_[0] = new QTNode(proot->minx_,
                                     proot->miny_,
                                     (proot->minx_ + proot->maxx_) /2 - 1,
                                     (proot->miny_ + proot->maxy_) /2 - 1,
                                     depth, proot);
    
    proot->children_[1] = new QTNode((proot->minx_ + proot->maxx_)/ 2,
                                     proot->miny_,
                                     proot->maxx_,
                                     (proot->miny_ + proot->maxy_)/ 2 - 1,
                                     depth, proot);
            
    proot->children_[2] = new QTNode((proot->minx_ + proot->maxx_)/ 2,
                                     (proot->miny_ + proot->maxy_)/ 2,
                                     proot->maxx_,
                                     proot->maxy_,
                                     depth, proot);
            
    proot->children_[3] = new QTNode(proot->minx_,
                                     (proot->miny_ + proot->maxy_)/ 2,
                                     (proot->minx_ + proot->maxx_)/ 2 - 1,
                                     proot->maxy_,
                                     depth, proot);
}

int QuadTree::index_f(QTNode * proot, GameObject * pObject)
{
    // quadrants look like:
    // ---------
    // | 0 | 1 |
    // ---------
    // | 3 | 2 |
    // ---------

    int ret = -1;

    int x = pObject->x();
    int y = pObject->y();
    int w = pObject->w();
    int h = pObject->h();

    int minx = proot->minx_;
    int miny = proot->miny_;
    int maxx = proot->maxx_;
    int maxy = proot->maxy_;


    if (minx <= x && x + w < (minx + maxx) / 2)
    {
        if (miny <= y && y + h < (miny + maxy) / 2 )
        {
            // point lies in quadrant 0
            ret = 0;
        }
        else if ((miny + maxy) / 2 < y && y + h <= maxy)
        {
            // point lies in quadrant 3
            ret = 3;
        }
    }
    else if ((minx + maxx) / 2 < x && x <= maxx)
    {
        if (miny <= y && y + h < (miny + maxy) / 2)
        {
            // point lies in quadrant 1
            ret = 1;
        }
        else if ((miny + maxy) / 2 < y && y + h <= maxy)
        {
            // point lies in quadrent 2
            ret = 2;
        }
    }

    return ret;                 
}

std::vector< GameObject * > QuadTree::which_child(QTNode * proot)
{
    std::vector< GameObject * > ret;
    for (int i = 0; i < proot->data_.size(); ++i)
    {
        int index = index_f(proot, proot->data_[i]);

        if (index == -1)
        {
            ret.push_back(proot->data_[i]);
        }
        else
        {
            insert(proot->children_[index], proot->data_[i]);            
        }
    }

    return ret;
}

void QuadTree::insert(QTNode * proot, GameObject * pObject)
{
    if(proot == NULL)
    {
        proot = new QTNode();
        proot->data_.push_back(pObject);
    }
    else
    {
        if (proot->children_[0] == NULL)
        {
            if (proot->data_.size() < proot->threshold_)
            {
                proot->data_.push_back(pObject);
            }
            else
            {
                proot->data_.push_back(pObject);
                make_children(proot);
                // decide where the data should go
                // and put the boundry data into the parent data
                proot->data_ = which_child(proot);
            }
        }
        else // proot has children
        {
            // handle data on teh boundries
            proot->data_.push_back(pObject);
            proot->data_ = which_child(proot);
        }
    }
}

QTNode * QuadTree::proot()
{
    return proot_;
}

void QuadTree::clear()
{
    remove(&proot_);
}


void QuadTree::remove(QTNode ** p)
{
    if (*p == NULL)
    {
        //do nothing
        return;
    }  
    else
    {
        remove(&(*p)->children_[0]);
        remove(&(*p)->children_[1]);
        remove(&(*p)->children_[2]);
        remove(&(*p)->children_[3]);

        delete *p;
        *p = NULL;
    }
}

int QuadTree::height(const QTNode * const proot)
{
    if (proot == NULL)
    {
        return -1;
    }
    else
    {
        return 1 + height(proot->children_[0]);
    }
}

std::ostream & operator<<(std::ostream & cout, const QTNode & q)
{
    cout << "<QTNode: " << &q
         // << ", minx_: " << q.minx_ << ", miny_: " << q.miny_
         // << ", maxx_: " << q.maxx_ << ", maxy_: " << q.maxy_
         << ", threshold: " << q.threshold_
         << ", data_.size(): " << q.data_.size()
         // << ", depth: " << q.depth_
         << ", parent: " <<  q.parent_
         << '>';
    
    return cout;
}

std::ostream & operator<<(std::ostream & cout, const QuadTree & qt)
{    
    return cout;
}

