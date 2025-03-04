#include "Controller.h"
#include "ModeController.h"
#include <QDebug>


Controller::Controller(Model *model, DrawingOpenGLWidget *view)
    : model(model), view(view)
{
    this->modeController = new ModeController(this, view);
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

void Controller::dummyDraw()
{

    static float offset = 0.1f;
    model->addLine(-0.5f + offset, -0.5f, 0.5f + offset, 0.5f);
    offset += 0.1f;
    // Trigger a repaint after adding a new line
    view->update();
}


// ----- internals -----
ModeController* Controller::getModeController()
{
    return modeController;
}

Model* Controller::getModel()
{
    return this->model;
}


// ----- Model Interactions -----
void Controller::addLine(float x1, float y1, float x2, float y2)
{
    model->addLine(x1,y1,x2,y2);
    view->update();
}

void Controller::addShape()
{
    model->addShape();
    view->update();
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



