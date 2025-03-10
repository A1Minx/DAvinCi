// Model.cpp
#include "Model.h"
#include <QDebug>

Model::Model()
{
    this->sqlServer = new SQLServer();
}


// ----- Interactions with persistent Data Model -----
void Model::addShape()
{
    qDebug() << "Shape added";
}

std::shared_ptr<Point> Model::addPoint(float x, float y, float z)
{
    int id = sqlServer->newPoint(x,y,z);
    std::shared_ptr<Point> point = std::make_shared<Point>(x,y,z, id);
    points.push_back(point);
    return point;
}

void Model::addLine(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2)
{
    lines.push_back(std::make_shared<Line>(p1, p2));
    sqlServer->newLine(p1->getID(), p2->getID());
}

// ----- Interactions with temporary Data -----
void Model::addTempLine(float x1, float y1, float z1, float x2, float y2, float z2)
{    
    tempLines.emplace_back(x1, y1, z1, x2, y2, z2);
}

void Model::removeTempLines()
{
    tempLines.clear();
}

void Model::addTempPoint(float x, float y, float z)
{
    markedPoints.emplace_back(x, y, z);
}

void Model::removeTempPoints()
{
    markedPoints.clear();
}

// ----- Getters -----
std::vector<std::shared_ptr<Point>> Model::getPoints() const
{
    return points;
}

std::vector<std::shared_ptr<Line>> Model::getLines() const
{
    return lines;
}