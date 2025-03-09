#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include "BasicObject.h"
#include <string>

class Point : BasicObject {

public:
    Point(float x, float y, float z, int id);
    ~Point();
    
    // Getter für die Koordinaten
    float getX() const { return x; }
    float getY() const { return y; }
    float getZ() const { return z; }

    int getID() const { return id; }

private:
    float x;
    float y;
    float z;
    std::string name;
    const int id; 
};

#endif // POINT_H_INCLUDED
