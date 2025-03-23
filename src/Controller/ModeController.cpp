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
    this->currentModeType = Modes::Selection;
}

void ModeController::changeMode(Modes mode)
{
    switch(mode) {
        case Modes::Selection:
            qDebug() << "entering Mode Selection";
            controller->setMouseTracking(true);
            this->CurrentMode = selection;
            this->currentModeType = Modes::Selection;
            break;
        case Modes::DrawLine:
            qDebug() << "entering Mode drawLine";
            controller->setMouseTracking(true);
            this->CurrentMode = drawLine;
            this->currentModeType = Modes::DrawLine;
            break;
        case Modes::DrawPoint:
            qDebug() << "entering Mode drawPoint";
            controller->setMouseTracking(false);
            this->CurrentMode = drawPoint;
            this->currentModeType = Modes::DrawPoint;
            break;
        default:
            qDebug() << "Mode not available";
    }
}

M_ModeInterface * ModeController::getCurrentMode()
{
    return this->CurrentMode;
}

void ModeController::reConfigureView()
{
    if ((this->currentModeType == Modes::DrawLine) || (this->currentModeType == Modes::Selection)) {
        controller->setMouseTracking(true);
    }
}