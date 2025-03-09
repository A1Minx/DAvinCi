#include "M_DrawPoint.h"
#include "Controller.h"
#include <QDebug>

M_DrawPoint::M_DrawPoint(Controller* controller)
    : controller(controller)
{
}

// Neue Methode mit Weltkoordinaten
void M_DrawPoint::onMouseClick(QMouseEvent *event, const QVector3D& worldPos) {
    if (event->button() == Qt::LeftButton) {
        qDebug() << "Left click at screen:" << event->x() << ", " << event->y();
        qDebug() << "World coordinates:" << worldPos.x() << ", " << worldPos.y() << ", " << worldPos.z();
        
        // Füge Punkt mit Weltkoordinaten hinzu
        controller->addPoint(worldPos.x(), worldPos.y(), worldPos.z());
    } else if (event->button() == Qt::RightButton) {
        controller->setModeSelection();
    }
}

