// Model.h
#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <memory>
#include "SQLServer.h"
#include "Point.h"
#include "Line.h"
#include "Spec.h"
#include "ComposedObject.h"
// Temporary lines dont have to be a (maybe in the future complex) Line object. Minimal Implementation:
struct TempLine {
    float x1, y1, z1;
    float x2, y2, z2;
    float color[4];
    float width;

    TempLine(float x1, float y1, float z1, float x2, float y2, float z2, float color[4], float width)
        : x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2), width(width) {
            for (int i = 0; i < 4; i++) {
                this->color[i] = color[i];
            }
        }
};

struct TempPoint {
    float x, y, z;
    float color[4];
    float size;

    TempPoint(float x, float y, float z, float color[4], float size) 
        : x(x), y(y), z(z), size(size) {
            for (int i = 0; i < 4; i++) {
                this->color[i] = color[i];
            }
        }
};

class Model {
public:
    Model();

    void addShape();
    void addLine(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2, std::shared_ptr<LineSpec> spec);
    std::shared_ptr<Point> addPoint(float x, float y, float z, std::shared_ptr<PointSpec> spec);

    void addTempLine(float x1, float y1, float z1, float x2, float y2, float z2, float color[4], float width);
    void removeTempLines();

    void addTempPoint(float x, float y, float z, float color[4], float size);
    void removeTempPoints();

    std::vector<std::shared_ptr<Point>> getPoints() const;
    std::vector<std::shared_ptr<Line>> getLines() const;
    std::vector<std::shared_ptr<ComposedObject>> getComposedObjects() const;
    const std::vector<TempLine>& getTempLines() const { return tempLines; }
    const std::vector<TempPoint>& getTempPoints() const { return markedPoints; }
    SQLServer *sqlServer;

    std::shared_ptr<PointSpec> getPointSpec(std::string name);
    std::shared_ptr<PointSpec> getPointSpec(int id);
    std::shared_ptr<LineSpec> getLineSpec(std::string name);
    std::shared_ptr<LineSpec> getLineSpec(int id);
    std::shared_ptr<PointSpec> addPointSpec(std::string name, float color[4], float size, int id);
    std::shared_ptr<LineSpec> addLineSpec(std::string name, float color[4], float width, int id);   
    std::shared_ptr<ComposedObject> getComposedObject(int id) const;
private:
    std::vector<std::shared_ptr<Point>> points;
    std::vector<std::shared_ptr<Line>> lines;
    std::vector<std::shared_ptr<ComposedObject>> composedObjects;
    std::vector<TempLine> tempLines;
    std::vector<TempPoint> markedPoints;

    std::vector<std::shared_ptr<PointSpec>> pointSpecs;
    std::vector<std::shared_ptr<LineSpec>> lineSpecs;
};

#endif // MODEL_H
