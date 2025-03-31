#include "Controller.h"
#include "ModeController.h"
#include <QDebug>
#include "View_OpenGLWidget.h"
#include "Orth_XY_OpenGLWidget.h"
#include "M_ModeInterface.h"



Controller::Controller(Model *model, View_OpenGLWidget *view)
    : model(model), view(view), modeController(nullptr)
{
    this->modeController = new ModeController(this);
    this->currPointSpec = model->getPointSpec(1);
    qDebug() << "currPointSpec: " << currPointSpec->getColor();
    this->currLineSpec = model->getLineSpec(2);
    qDebug() << "currLineSpec: " << currLineSpec->getColor();
}

void Controller::setView(View_OpenGLWidget *view)
{
    this->view = view;
}

void Controller::setHiddenAxis(char axis) {
    switch (axis) {
        case 'x':
            hiddenAxis = 'x';
            break;
        case 'y':
            hiddenAxis = 'y';
            break;
        case 'z':
            hiddenAxis = 'z';
            break;
        case '0':
            hiddenAxis = '0';
            break;
        default:
            qDebug() << "Invalid hidden axis";
            break;
    }
}



// ----- GUI controlls -----
// -- Data Management
void Controller::save()
{
    qDebug() << "save Button clicked";
}

void Controller::load()
{
    qDebug() << "load Button clicked";
}


// -- Interaction
void Controller::clearShapes()
{
    qDebug() << "clearing Shapes implementation";
}

void Controller::setModeDrawPoint()
{
    qDebug() << "setMode DrawPoint implementation";

    modeController->changeMode(ModeController::Modes::DrawPoint);
}
void Controller::setModeDrawLine()
{

    qDebug() << "setMode DrawLine implementation";

    modeController->changeMode(ModeController::Modes::DrawLine);
}

void Controller::setModeSelection()
{
    qDebug() << "setMode Selection implementation";

    modeController->changeMode(ModeController::Modes::Selection);
}


// ----- internals -----
ModeController* Controller::getModeController()
{
    return modeController;
}


// -- Calculations --
std::shared_ptr<Point> Controller::getNearestPoint(float x, float y, float z)
// gets the nearest Point
{
    qDebug() << "getNearestPoint for x: " << x << " y: " << y << " z: " << z;
    static const float MAX_DISTANCE = 20.0f;
    std::shared_ptr<Point> candidate = nullptr;
    float minDistance = MAX_DISTANCE;

    for (const std::shared_ptr<Point>& point : model->getPoints()) {
        float distance = point->distanceTo(x, y, z);
        if (distance < minDistance) {
            minDistance = distance;
            candidate = point;
        }
    }
    return candidate;
}

std::shared_ptr<Point> Controller::getNearestPoint(float a, float b, char hiddenAxis) 
{
// overload to get nearest points that are further away on the hidden axis
// TODO: Implement a function to get the point that is closest on the invisible axis after the check of the visible axes
    static const float MAX_DISTANCE = 20.0f;
    std::vector<std::pair<float, std::shared_ptr<Point>>> candidates; //float value = distance to current horizon in the hidden axis
    float minDistance = MAX_DISTANCE;

    for (const std::shared_ptr<Point>& point : model->getPoints()) {
        float distance;
        float distanceToHorizon;
        switch (hiddenAxis) {
            case 'x':
                distance = point->distanceTo(point->getX(), a, b);
                distanceToHorizon = point->distanceToHorizon(view->getHorizon(), point->getX());
                break;
            case 'y':
                distance = point->distanceTo(a, point->getY(), b);
                distanceToHorizon = point->distanceToHorizon(view->getHorizon(), point->getY());
                break;
            case 'z':
                distance = point->distanceTo(a, b, point->getZ());
                distanceToHorizon = point->distanceToHorizon(view->getHorizon(), point->getZ());
                break;
            default:
                qDebug() << "Invalid hidden axis";
                break;
        }
        if ((distance < minDistance) or (distance == minDistance)) {
            minDistance = distance;
            candidates.push_back(std::make_pair(distanceToHorizon, point));
        }
    }

    // if multiple points in a,b proximity are return candidates, return the one closest to the current horizon
    // TODO: test behaviour with changing horizons in View overlapping drawing
    if (candidates.size() == 1) { 
        qDebug() << "One candidate found: " << candidates[0].second->getX() << ", " << candidates[0].second->getY() << ", " << candidates[0].second->getZ();
        return candidates[0].second;
    } 
    else if (candidates.size() > 1) {
        qDebug() << "Multiple candidates found:";
        for (const auto& cand_pair : candidates) {
            qDebug() << "Candidate: " << cand_pair.first << " | " << cand_pair.second->getX() << ", " << cand_pair.second->getY() << ", " << cand_pair.second->getZ();
        }
        std::shared_ptr<Point> candidate = candidates[0].second;
        float smallestDistanceToHorizon = candidates[0].first;
        for (const auto& cand_pair : candidates) {
            if (cand_pair.first < smallestDistanceToHorizon) {
                smallestDistanceToHorizon = cand_pair.first;
                candidate = cand_pair.second;
            }
        }
        return candidate;
        //TODO: check if its faster to keep only the smallest distanceToHorizon Point in the loop above
    }
    else {
        qDebug() << "No candidates found";
        return nullptr;
    }
}



void Controller::addShape()
{
    //TODO: Check if this can be used for all types of Shapes (Polymorphism)
    model->addShape();
    view->update();
}

void Controller::addLine(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2, std::shared_ptr<LineSpec> spec)
{
    model->addLine(p1, p2, spec);
    view->update();
}

std::shared_ptr<Point> Controller::addPoint(float x, float y, float z, std::shared_ptr<PointSpec> spec)
{
    std::shared_ptr<Point> point;

    switch (hiddenAxis) {
        case 'x':
            point = model->addPoint(view->getHorizon(),y,z, spec);
            view->update();
            return point;
        case 'y':
            point = model->addPoint(x,view->getHorizon(),z, spec);
            view->update();
            return point;
        case 'z':
            point = model->addPoint(x,y,view->getHorizon(), spec);
            view->update();
            return point;
        case '0':
            qDebug() << "not implemented";
            return nullptr;
        default:
            qDebug() << "Invalid hidden axis";
            return nullptr;
    }
}



// ----- SQL Test -----
void Controller::readSQLLines()
{
    model->sqlServer->readSQLLines();
}

void Controller::readSQLPoints()
{
    model->sqlServer->readSQLPoints();
}


// -- change View Options --
void Controller::setMouseTracking(bool tracking)
{
    view->setMouseTracking(tracking);
};



// ----- Interactivity -----
void Controller::handleMouseClick(QMouseEvent *event)
{
    // Bildschirmkoordinaten in Weltkoordinaten konvertieren. Notwendig, da ansonsten nicht an der Mausposition gezeichnet wird. 
    // TODO: Prüfen ob es ein Problem ist, dass Screen Koordinaten int und Weltkoordinaten float sind. Genauigkeitsverlust? 
    QVector3D worldPos = view->screenToWorld(event->x(), event->y());
    modeController->getCurrentMode()->onMouseClick(event, worldPos);
}

void Controller::handleMouseMove(QMouseEvent *event)
{
    QVector3D worldPos = view->screenToWorld(event->x(), event->y());
    modeController->getCurrentMode()->onMouseMove(event, worldPos);
}


// -- Markers and Temporary elements --
void Controller::addTempLine(float x1, float y1, float z1, float x2, float y2, float z2, float color[4], float width)
{
    model->addTempLine(x1, y1, z1, x2, y2, z2, color, width);
    view->update();
}

void Controller::removeTempLines()
{
    model->removeTempLines();
    view->update();
}

void Controller::addTempPoint(float x, float y, float z, float color[4], float size)
{
    model->addTempPoint(x, y, z, color, size);
    view->update();
}

void Controller::removeTempPoints()
{
    model->removeTempPoints();
    view->update();
}


// ----- Spec Management -----

void Controller::setCurrPointSpec(std::shared_ptr<PointSpec> spec)
{
    currPointSpec = spec;
}

void Controller::setCurrLineSpec(std::shared_ptr<LineSpec> spec)
{
    currLineSpec = spec;
}

std::shared_ptr<PointSpec> Controller::getCurrPointSpec()
{
    return currPointSpec;
}

std::shared_ptr<LineSpec> Controller::getCurrLineSpec()
{
    return currLineSpec;
}