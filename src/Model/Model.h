// Model.h
#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "SQLServer.h"
#include "Point.h"

class Model {
public:
    Model();


    void addLine(float x1, float y1, float x2, float y2);

    void addShape();

    void addPoint(float x, float y, float z);

    SQLServer *sqlServer;


    const std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>>& getLines() const;

private:
    // Store multiple lines as pairs of coordinates
    std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>> lines;

    std::vector<Point*> points;
};

#endif // MODEL_H
