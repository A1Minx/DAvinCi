#include "M_Selection.h"
#include "Controller.h"
#include <QDebug>

M_Selection::M_Selection(Controller* controller)
    : controller(controller)
{

}



void M_Selection::onMouseClick(QMouseEvent *event) {

    float x = event->x();
    float y = event->y();

    if (event->button() == Qt::MiddleButton) {
        qDebug() << "Wheel Click at " << x << ", " << y;
    }
}


