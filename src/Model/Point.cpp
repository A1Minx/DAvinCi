#include "Point.h"
#include <cmath>
#include <QDebug>

Point::Point(float x, float y, float z, std::shared_ptr<PointSpec> spec, int id)
    : x(x), y(y), z(z), id(id)
{
    this->spec = spec;
}

Point::~Point()
{
}

float Point::distanceTo(float x, float y, float z)
{
    return sqrt(pow(this->x - x, 2) + pow(this->y - y, 2) + pow(this->z - z, 2));
    qDebug() << "checking point: " << this->x << ", " << this->y << ", " << this->z;
}


float Point::distanceToHorizon(float horizon, float axisCoordinate)
{
    return fabs(axisCoordinate - horizon);
}


