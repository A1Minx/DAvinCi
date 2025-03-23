#include "M_DrawPoint.h"
#include "Controller.h"
#include <QDebug>

M_DrawPoint::M_DrawPoint(Controller* controller)
    : controller(controller)
{
}

void M_DrawPoint::onMouseClick(QMouseEvent *event, const QVector3D& worldPos) {
    if (event->button() == Qt::LeftButton) {
        qDebug() << "Left click at screen:" << event->x() << ", " << event->y();
        qDebug() << "World coordinates:" << worldPos.x() << ", " << worldPos.y() << ", " << worldPos.z();
        
        controller->addPoint(worldPos.x(), worldPos.y(), worldPos.z(), controller->getCurrPointSpec());
    } else if (event->button() == Qt::RightButton) {
        controller->setModeSelection();
    }
}

void M_DrawPoint::onMouseMove(QMouseEvent *event, const QVector3D& worldPos) {
    // Nothing to do here; empty implementation to satisfy Interface
    // TODO: Check if this is a good design principle. 
    // Pro Argument: All Modes implement the necessary Mouse Methods, and it doesnt hurt if they are empty.
    qDebug() << "Mouse Move at screen:" << event->x() << ", " << event->y();
    qDebug() << "World coordinates:" << worldPos.x() << ", " << worldPos.y() << ", " << worldPos.z();

    return;
}