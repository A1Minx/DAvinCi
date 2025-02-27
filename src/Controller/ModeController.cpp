#include "ModeController.h"
#include <QDebug>

ModeController::ModeController(Controller *controller, DrawingOpenGLWidget *view)
    :controller(controller), view(view)
{
    this->drawLine = new M_DrawLine(controller);
    this->drawPoint = new M_DrawPoint(controller);
}

void ModeController::changeMode(Modes mode)
{
    qDebug() << "Changed mode";
    switch(mode) {
        case Modes::DrawLine:
            this->CurrentMode = drawLine;
            break;
        case Modes::DrawPoint:
            this->CurrentMode = drawPoint;
            break;
        default:
            qDebug() << "Mode not available";
    }
}

M_ModeInterface * ModeController::getCurrentMode()
{
    qDebug() << "returning Mode";
    return this->CurrentMode;
}
