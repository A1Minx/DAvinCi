#include "M_Selection.h"
#include "Controller.h"
#include <QDebug>

M_Selection::M_Selection(Controller* controller)
    : M_ModeInterface(controller)
{
}

void M_Selection::onMouseClick(QMouseEvent *event, const QVector3D& worldPos) {
    if (event->button() == Qt::MiddleButton) {
        qDebug() << "Wheel Click at screen:" << event->x() << ", " << event->y();
        qDebug() << "World coordinates:" << worldPos.x() << ", " << worldPos.y() << ", " << worldPos.z();
    }
}

void M_Selection::onMouseMove(QMouseEvent *event, const QVector3D& worldPos) {
    nearestPoint = nearestPointSelection(event, worldPos, controller->getHiddenAxis());
    if (nearestPoint) {
        controller->removeTempPoints();
        controller->addTempPoint(nearestPoint->getX(), nearestPoint->getY(), nearestPoint->getZ(), tempColor, tempSize);
    } else {
        controller->removeTempPoints();
    }
}


