// Model.h
#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <memory>
#include "SQLServer.h"
#include "Point.h"
#include "Line.h"

// Temporary lines dont have to be a (maybe in the future complex) Line object. Minimal Implementation:
struct TempLine {
    float x1, y1, z1;
    float x2, y2, z2;
    
    TempLine(float x1, float y1, float z1, float x2, float y2, float z2)
        : x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2) {}
};

struct TempPoint {
    float x, y, z;
    
    TempPoint(float x, float y, float z) : x(x), y(y), z(z) {}
};

class Model {
public:
    Model();

    void addShape();
    void addLine(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2);
    std::shared_ptr<Point> addPoint(float x, float y, float z);

    void addTempLine(float x1, float y1, float z1, float x2, float y2, float z2);
    void removeTempLines();

    void addTempPoint(float x, float y, float z);
    void removeTempPoints();

    std::vector<std::shared_ptr<Point>> getPoints() const;
    std::vector<std::shared_ptr<Line>> getLines() const;
    const std::vector<TempLine>& getTempLines() const { return tempLines; }
    const std::vector<TempPoint>& getTempPoints() const { return markedPoints; }
    SQLServer *sqlServer;

private:
    std::vector<std::shared_ptr<Point>> points;
    std::vector<std::shared_ptr<Line>> lines;
    std::vector<TempLine> tempLines;
    std::vector<TempPoint> markedPoints;
};

#endif // MODEL_H
