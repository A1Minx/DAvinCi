#include "Line.h"

Line::Line(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2, std::shared_ptr<LineSpec> spec)
    : p1(p1), p2(p2), 
{
    calculateLength();
}

Line::~Line()
{
    
}

void Line::calculateLength()
{
    length = p1->distanceTo(p2->getX(), p2->getY(), p2->getZ());
}

