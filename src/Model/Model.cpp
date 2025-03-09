// Model.cpp
#include "Model.h"
#include <QDebug>

Model::Model()
{
    this->sqlServer = new SQLServer();
}

void Model::addShape()
{
    qDebug() << "Shape added";
}

Point* Model::addPoint(float x, float y, float z)
{
    int id = sqlServer->newPoint(x,y,z);
    Point* point = new Point(x,y,z, id);
    points.push_back(point);
    return point;
}

void Model::addLine(Point* p1, Point* p2)
{
    lines.push_back(new Line(p1, p2));
    sqlServer->newLine(p1->getID(), p2->getID());
}


// ----- Getters -----
std::vector<std::shared_ptr<Point>> Model::getPoints() const
{
    std::vector<std::shared_ptr<Point>> result;
    for (Point* point : points) {
        result.push_back(std::shared_ptr<Point>(point, [](Point*) {}));
    }
    return result;
}

std::vector<std::shared_ptr<Line>> Model::getLines() const
{
    std::vector<std::shared_ptr<Line>> result;
    for (Line* line : lines) {
        result.push_back(std::shared_ptr<Line>(line, [](Line*) {}));
    }
    return result;
}