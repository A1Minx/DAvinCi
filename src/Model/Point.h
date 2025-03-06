#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include "BasicObject.h"
#include <string>

class Point : BasicObject {

public:
    Point(float x, float y, float z);
    ~Point();

    void draw();

private:

    float x;
    float y;
    float z;
    std::string name;
};

#endif // POINT_H_INCLUDED
