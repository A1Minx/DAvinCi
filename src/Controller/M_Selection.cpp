#include "M_Selection.h"
#include "Controller.h"
#include <QDebug>

M_Selection::M_Selection(Controller* controller)
    : controller(controller)
{
}

void M_Selection::onMouseClick(QMouseEvent *event, const QVector3D& worldPos) {
    if (event->button() == Qt::MiddleButton) {
        qDebug() << "Wheel Click at screen:" << event->x() << ", " << event->y();
        qDebug() << "World coordinates:" << worldPos.x() << ", " << worldPos.y() << ", " << worldPos.z();
    }
}

void M_Selection::onMouseMove(QMouseEvent *event, const QVector3D& worldPos) {

}


