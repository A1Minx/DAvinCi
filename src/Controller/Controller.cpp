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
    qDebug() << "getNearestPoint for hidden axis: " << hiddenAxis << " with a: " << a << " and b: " << b;
// overload to get nearest points that are further away on the hidden axis
// TODO: Implement a function to get the point that is closest on the invisible axis after the check of the visible axes
    static const float MAX_DISTANCE = 20.0f;
    std::shared_ptr<Point> candidate = nullptr;
    float minDistance = MAX_DISTANCE;

    for (const std::shared_ptr<Point>& point : model->getPoints()) {
        float distance;
        switch (hiddenAxis) {
            case 'x':
                distance = point->distanceTo(point->getX(), a, b);
                break;
            case 'y':
                distance = point->distanceTo(a, point->getY(), b);
                break;
            case 'z':
                distance = point->distanceTo(a, b, point->getZ());
                break;
            default:
                qDebug() << "Invalid hidden axis";
                break;
        }
        qDebug() << "distance: " << distance;
        if (distance < minDistance) {
            minDistance = distance;
            candidate = point;
        }
    }
    return candidate;
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
    std::shared_ptr<Point> point = model->addPoint(x,y,z, spec);
    view->update();
    return point;
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