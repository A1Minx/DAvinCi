#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include "BasicObject.h"
#include <string>
#include "Spec.h"
#include <memory>
class Point : public BasicObject {

public:
    Point(float x, float y, float z, std::shared_ptr<PointSpec> spec, int id);

    ~Point();
    
    // Getter für die Koordinaten
    float getX() const { return x; }
    float getY() const { return y; }
    float getZ() const { return z; }

    int getID() const { return id; }

    float distanceTo(float x, float y, float z);
    float distanceToHorizon(float horizon, float axisCoordinate);
    std::shared_ptr<PointSpec> getSpec() const { return spec; }
    void setSpec(std::shared_ptr<PointSpec> spec) { this->spec = spec; }
    
    float* getColor() const { return spec->getColor(); }
    float getSize() const { return spec->getSize(); }

private:
    float x;
    float y;
    float z;
    std::string name;

    const int id; 

    std::shared_ptr<PointSpec> spec;
};

#endif // POINT_H_INCLUDED
