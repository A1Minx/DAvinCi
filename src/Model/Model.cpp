// Model.cpp
#include "Model.h"
#include <QDebug>

Model::Model()
{
    this->sqlServer = new SQLServer(this);

    //TODO: Update Local Model when SQL Server is updated from different client
    
    pointSpecs = sqlServer->readSQLPointSpec();
    lineSpecs = sqlServer->readSQLLineSpec();
    composedObjects = sqlServer->readSQLRootComposedObjects();
    
}


// ----- Interactions with persistent Data Model -----
void Model::addShape()
{
    qDebug() << "Shape added";
}

std::shared_ptr<PointSpec> Model::addPointSpec(std::string name, float color[4], float size, int id)
{
    std::shared_ptr<PointSpec> spec = std::make_shared<PointSpec>(name, color, size, id);
    pointSpecs.push_back(spec);
    return spec;
}

std::shared_ptr<LineSpec> Model::addLineSpec(std::string name, float color[4], float width, int id)
{
    std::shared_ptr<LineSpec> spec = std::make_shared<LineSpec>(name, color, width, id);
    lineSpecs.push_back(spec);
    return spec;
}

std::shared_ptr<Point> Model::addPoint(float x, float y, float z, std::shared_ptr<PointSpec> spec, int parent_id)
{
    int id = sqlServer->newPoint(x,y,z, spec, parent_id);
    std::shared_ptr<Point> point = std::make_shared<Point>(x,y,z, spec, id);
    for (std::shared_ptr<ComposedObject> composedObject : composedObjects) {
        if (composedObject->getID() == parent_id) {
            composedObject->addPoint(point);
            break;
        }
    }
    return point;
}

void Model::addLine(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2, std::shared_ptr<LineSpec> spec, int parent_id)
{
    sqlServer->newLine(p1->getID(), p2->getID(), spec, parent_id);
    std::shared_ptr<Line> line = std::make_shared<Line>(p1, p2, spec);
    for (std::shared_ptr<ComposedObject> composedObject : composedObjects) {
        if (composedObject->getID() == parent_id) {
            composedObject->addLine(line);
            break;
        }
    }
}

// ----- Interactions with temporary Data -----
void Model::addTempLine(float x1, float y1, float z1, float x2, float y2, float z2, float color[4], float width)
{    
    tempLines.emplace_back(x1, y1, z1, x2, y2, z2, color, width);
}

void Model::removeTempLines()
{
    tempLines.clear();
}

void Model::addTempPoint(float x, float y, float z, float color[4], float size)
{
    markedPoints.emplace_back(x, y, z, color, size);
}

void Model::removeTempPoints()
{
    markedPoints.clear();
}

// ----- Getters -----
//TODO: Only do a full SQL read if necessary. implement update protocol
std::vector<std::shared_ptr<Point>> Model::getPoints() const
{
    std::vector<std::shared_ptr<Point>> points = sqlServer->readSQLPoints();
    return points;
}

std::vector<std::shared_ptr<Line>> Model::getLines() const
{
    std::vector<std::shared_ptr<Line>> lines = sqlServer->readSQLLines();
    return lines;
}

std::vector<std::shared_ptr<ComposedObject>> Model::getComposedObjects() const
{
    std::vector<std::shared_ptr<ComposedObject>> composedObjects = sqlServer->readSQLRootComposedObjects();
    return composedObjects;
}

std::shared_ptr<ComposedObject> Model::getComposedObject(int id) const
{
    for (std::shared_ptr<ComposedObject> composedObject : composedObjects)
    {
        if (composedObject->getID() == id) return composedObject;
    }
    return nullptr;
}

std::shared_ptr<PointSpec> Model::getPointSpec(std::string name)
{
    for (std::shared_ptr<PointSpec> spec : pointSpecs)
    {
        if (spec->getName() == name) return spec;
    }
    qDebug() << "Spec not found";
    return nullptr;
}

std::shared_ptr<PointSpec> Model::getPointSpec(int id)
{
    for (std::shared_ptr<PointSpec> spec : pointSpecs)
    {
        if (spec->getID() == id) return spec;
    }
    qDebug() << "Spec not found";
    return nullptr;
}   

std::shared_ptr<LineSpec> Model::getLineSpec(int id)
{
    for (std::shared_ptr<LineSpec> spec : lineSpecs)
    {
        if (spec->getID() == id) return spec;
    }
    qDebug() << "Spec not found";
    return nullptr;
}

std::shared_ptr<LineSpec> Model::getLineSpec(std::string name)
{
    for (std::shared_ptr<LineSpec> spec : lineSpecs)
    {
        if (spec->getName() == name) return spec;
    }
    qDebug() << "Spec not found";
    return nullptr;
}

