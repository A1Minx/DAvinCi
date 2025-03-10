#include "Controller.h"
#include "ModeController.h"
#include <QDebug>


Controller::Controller(Model *model, DrawingOpenGLWidget *view)
    : model(model), view(view)
{
    this->modeController = new ModeController(this);
}

void Controller::setView(DrawingOpenGLWidget *view)
{
    this->view = view;
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
{
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



void Controller::addShape()
{
    //TODO: Check if this can be used for all types of Shapes (Polymorphism)
    model->addShape();
    view->update();
}

void Controller::addLine(Point *p1, Point *p2)
{
    model->addLine(p1, p2);
    view->update();
}

Point* Controller::addPoint(float x, float y, float z)
{
    Point* point = model->addPoint(x,y,z);
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
void Controller::addTempLine(float x1, float y1, float z1, float x2, float y2, float z2)
{
    model->addTempLine(x1, y1, z1, x2, y2, z2);
    view->update();
}

void Controller::removeTempLines()
{
    model->removeTempLines();
    view->update();
}

void Controller::addTempPoint(float x, float y, float z)
{
    model->addTempPoint(x, y, z);
    view->update();
}

void Controller::removeTempPoints()
{
    model->removeTempPoints();
    view->update();
}