#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

#include "BasicObject.h"
#include "Point.h"
#include <memory>
class Line : BasicObject {

public:
    Line(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2);
    ~Line();

    std::shared_ptr<Point> getP1() const { return p1; }
    std::shared_ptr<Point> getP2() const { return p2; } 

private:
    std::shared_ptr<Point> p1;
    std::shared_ptr<Point> p2;
};

#endif // LINE_H_INCLUDED
