#include "ModeController.h"
#include "Controller.h"
#include <QDebug>

ModeController::ModeController(Controller *controller)
    :controller(controller)
{
    this->drawLine = new M_DrawLine(controller);
    this->drawPoint = new M_DrawPoint(controller);
    this->selection = new M_Selection(controller);

    this->CurrentMode = selection; // set default mode
}

void ModeController::changeMode(Modes mode)
{
    switch(mode) {
        case Modes::Selection:
            qDebug() << "entering Mode Selection";
            controller->setMouseTracking(true);
            this->CurrentMode = selection;
            break;
        case Modes::DrawLine:
            qDebug() << "entering Mode drawLine";
            controller->setMouseTracking(true);
            this->CurrentMode = drawLine;
            break;
        case Modes::DrawPoint:
            qDebug() << "entering Mode drawPoint";
            controller->setMouseTracking(false);
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
