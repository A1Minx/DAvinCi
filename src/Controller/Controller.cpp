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
void Controller::readSQL()
{
    model->sqlServer->readSQL();
}

void Controller::writeSQL()
{
    const char *dummyParam[2] = {"10", "20"};
    model->sqlServer->writeSQL(dummyParam);
}



// ----- Interactivity -----
void Controller::handleMouseClick(QMouseEvent *event)
{
    // Bildschirmkoordinaten in Weltkoordinaten konvertieren. Notwendig, da ansonsten nicht an der Mausposition gezeichnet wird. 
    // TODO: Prüfen ob es ein Problem ist, dass Screen Koordinaten int und Weltkoordinaten float sind. Genauigkeitsverlust? 
    QVector3D worldPos = view->screenToWorld(event->x(), event->y());
    
    modeController->getCurrentMode()->onMouseClick(event, worldPos);
}



