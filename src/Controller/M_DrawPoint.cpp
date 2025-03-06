#include "M_DrawPoint.h"
#include "Controller.h"
#include <QDebug>

M_DrawPoint::M_DrawPoint(Controller* controller)
    : controller(controller)
{

}



void M_DrawPoint::onMouseClick(QMouseEvent *event) {

    float x = event->x();
    float y = event->y();

    if (event->button() == Qt::LeftButton) {

        qDebug() << "Left click at " << x << ", " << y;
        qDebug() << "Draw Point";qDebug() << "returning Mode";
        controller->addPoint(x,y,0);
    } else if (event->button() == Qt::RightButton) {
        controller->getModeController()->changeMode(ModeController::Modes::Selection);

    }


}

