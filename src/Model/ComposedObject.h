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

    ComposedObject(int id, std::string name, std::shared_ptr<ComposedObject> parent, int parent_id);
    ~ComposedObject();

    void addChild(std::shared_ptr<ComposedObject> child) { children.push_back(child); }
    void removeChild(std::shared_ptr<ComposedObject> child) { children.erase(::std::remove(children.begin(), children.end(), child), children.end()); }
    std::vector<std::shared_ptr<ComposedObject>> getChildren() const { return children; }

    void addPoint(std::shared_ptr<Point> point) { points.push_back(point); }
    void removePoint(std::shared_ptr<Point> point) { points.erase(::std::remove(points.begin(), points.end(), point), points.end()); }
    std::vector<std::shared_ptr<Point>> getPoints() const { return points; }

    void addLine(std::shared_ptr<Line> line) { lines.push_back(line); }
    void removeLine(std::shared_ptr<Line> line) { lines.erase(::std::remove(lines.begin(), lines.end(), line), lines.end()); }
    std::vector<std::shared_ptr<Line>> getLines() const { return lines; }

    void setName(std::string name) { this->name = name; }
    std::string getName() { return name; }

    void setParent(std::shared_ptr<ComposedObject> parent) { this->parent = parent; }
    std::shared_ptr<ComposedObject> getParent() { return parent; }

    int getID() { return id; }
    int getParentID() { return parent_id; }



protected:

    std::string name;
    int id;
    int parent_id;

    std::shared_ptr<ComposedObject> parent;
    std::vector<std::shared_ptr<ComposedObject>> children;
    std::vector<std::shared_ptr<Point>> points;
    std::vector<std::shared_ptr<Line>> lines;

};

#endif // COMPOSED_OBJECT_H_INCLUDED
