// Model.h
#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <memory>
#include "SQLServer.h"
#include "Point.h"
#include "Line.h"

class Model {
public:
    Model();

    void addShape();
    void addLine(Point* p1, Point* p2);
    Point* addPoint(float x, float y, float z);

    std::vector<std::shared_ptr<Point>> getPoints() const;
    std::vector<std::shared_ptr<Line>> getLines() const;
    SQLServer *sqlServer;

private:
    std::vector<Point*> points;
    std::vector<Line*> lines;
};

#endif // MODEL_H
