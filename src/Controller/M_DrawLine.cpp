#include "M_DrawLine.h"
#include "Controller.h"
#include <QDebug>

M_DrawLine::M_DrawLine(Controller* controller)
    : controller(controller)
{

}



void M_DrawLine::onMouseClick(QMouseEvent *event) {

    // Normalize mouse coordinates to OpenGL (-1 to 1 range)
    float x = event->x();
    float y = event->y();

    if (firstClick) {
        qDebug() << "Draw Line First Phase";
        x1 = x;
        y1 = y;
        firstClick = false;
    } else {
        qDebug() << "Draw Line Second Phase";
        //controller->addLine(x1, y1, x, y);
        controller->addShape();
        firstClick = true;
    }
}

