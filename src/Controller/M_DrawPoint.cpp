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



    qDebug() << "Draw Point";
    controller->addShape();
    //controller->addLine(x-5, y-5, x+5, y+5);
    //controller->addLine(x-5, y+5, x+5, y-5);
}

