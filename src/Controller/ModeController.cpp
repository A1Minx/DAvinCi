#include "ModeController.h"
#include <QDebug>

ModeController::ModeController(Controller *controller, DrawingOpenGLWidget *view)
    :controller(controller), view(view)
{
    this->drawLine = new M_DrawLine(controller);
    this->drawPoint = new M_DrawPoint(controller);
    this->selection = new M_Selection(controller);
}

void ModeController::changeMode(Modes mode)
{
    switch(mode) {
        case Modes::Selection:
            qDebug() << "entering Mode Selection";
            this->CurrentMode = selection;
            break;
        case Modes::DrawLine:
            qDebug() << "entering Mode drawLine";
            this->CurrentMode = drawLine;
            break;
        case Modes::DrawPoint:
            qDebug() << "entering Mode drawPoint";
            this->CurrentMode = drawPoint;
            break;
        default:
            qDebug() << "Mode not available";
    }
}

M_ModeInterface * ModeController::getCurrentMode()
{
    return this->CurrentMode;
}
