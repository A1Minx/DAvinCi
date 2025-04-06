#ifndef COMPOSED_OBJECT_H_INCLUDED
#define COMPOSED_OBJECT_H_INCLUDED

#include "Object3D.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>
#include "Point.h"
#include "Line.h"
#include "Object3D.h"


class ComposedObject : public Object3D {

public:

    ComposedObject(std::string name, Object3D* parent);
    ~ComposedObject();

    void addChild(Object3D* child) { children.push_back(child); }
    void removeChild(Object3D* child) { children.erase(::std::remove(children.begin(), children.end(), child), children.end()); }
    std::vector<Object3D*> getChildren() const { return children; }

    void addPoint(Point* point) { points.push_back(point); }
    void removePoint(Point* point) { points.erase(::std::remove(points.begin(), points.end(), point), points.end()); }
    std::vector<Point*> getPoints() const { return points; }

    void addLine(Line* line) { lines.push_back(line); }
    void removeLine(Line* line) { lines.erase(::std::remove(lines.begin(), lines.end(), line), lines.end()); }
    std::vector<Line*> getLines() const { return lines; }

    void setName(std::string name) { this->name = name; }
    std::string getName() { return name; }

    void setParent(Object3D* parent) { this->parent = parent; }
    Object3D* getParent() { return parent; }




protected:

    std::string name;

    Object3D* parent;
    std::vector<Object3D*> children;
    std::vector<Point*> points;
    std::vector<Line*> lines;

};

#endif // COMPOSED_OBJECT_H_INCLUDED
