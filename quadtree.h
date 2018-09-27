#ifndef QUADTREE
#define QUADTREE

#include <iostream>
#include <vector>
#include "gameobject.h"
#include "Surface.h"

class QTNode
{
public:
    QTNode (int minx=0, int miny=0, int maxx=640, int maxy=480,
            int depth=0, QTNode * parent=NULL)
        :minx_(minx), miny_(miny), maxx_(maxx), maxy_(maxy),
        depth_(depth), parent_(parent)
    {
        for (int i = 0; i < 4; ++i)
        {
            children_[i] = NULL;
        }
    }

    
    int minx_, miny_, maxx_, maxy_;
    QTNode * children_[4];
    static int threshold_;
    std::vector< GameObject * > data_;
    int depth_;
    QTNode * parent_;
};

std::ostream & operator<<(std::ostream &, const QTNode &);

class QuadTree
{
public:
    QuadTree()
        :proot_(NULL)
    {}
    
    QuadTree (QTNode * proot=NULL)
        :proot_(proot)
    {}

    void who_collides(QTNode * proot,
                                std::vector< GameObject * > &collision_v);
    std::vector< GameObject * > which_child(QTNode * proot);
    int index_f(QTNode * proot, GameObject * pObject);
    void make_children(QTNode * proot);
    void insert(QTNode * proot, GameObject * p);
    void clear();
    void remove(QTNode ** p);
    int height(const QTNode * const proot);
    QTNode * proot();        
    
private:
    QTNode * proot_;
};

std::ostream & operator<<(std::ostream &, const QuadTree &);

bool check_collision(GameObject * good_guy, GameObject *  bad_guy);
void preorder_qt_print(QTNode * proot);
void create_line(std::vector< Line > &l, QTNode * proot);

#endif

