// Model.cpp
#include "Model.h"
#include <QDebug>

Model::Model()
{
    this->sqlServer = new SQLServer();
}

void Model::addLine(float x1, float y1, float x2, float y2) {
    // Add a new line to the vector
    lines.push_back({{x1, y1}, {x2, y2}});
}

void Model::addShape()
{
    qDebug() << "Shape added";
}

void Model::addPoint(float x, float y, float z)
{
    points.push_back(new Point(x,y,z));
    sqlServer->newPoint(x,y,z);
}



const std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>>& Model::getLines() const {
    return lines;
}
