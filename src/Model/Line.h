#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

#include "BasicObject.h"
#include "Point.h"

class Line : BasicObject {

public:
    Line(Point* p1, Point* p2);
    ~Line();

    Point* getP1() const { return p1; }
    Point* getP2() const { return p2; } 

private:
    Point* p1;
    Point* p2;
};

#endif // LINE_H_INCLUDED
