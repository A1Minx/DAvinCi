#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

#include "BasicObject.h"
#include "Point.h"
#include <memory>
#include <cstddef>
#include "Spec.h"
class Line : public BasicObject {

public:
    Line(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2, std::shared_ptr<LineSpec> spec);
    ~Line();

    std::shared_ptr<Point> getP1() const { return p1; }
    std::shared_ptr<Point> getP2() const { return p2; } 
    float getLength() const { return length; }
    
    std::shared_ptr<LineSpec> getSpec() const { return spec; }
    void setSpec(std::shared_ptr<LineSpec> spec) { this->spec = spec; }

    float* getColor() const { return spec->getColor(); }
    float getWidth() const { return spec->getWidth(); }
    
private:
    std::shared_ptr<Point> p1;
    std::shared_ptr<Point> p2;
    float length;

    void calculateLength();

    std::shared_ptr<LineSpec> spec;
};

#endif // LINE_H_INCLUDED
