#include <iostream>
#include "gameobject.h"

std::ostream & operator<<(std::ostream & cout, Line & l)
{
    cout << "line: " << '(' << l.x0() << ',' << l.y0()
         << ")<->(" << l.x1() << ',' << l.y1();
    
    return cout;
}
