#include "M_DrawLine.h"
#include "Controller.h"
#include "Point.h"
#include <QDebug>

M_DrawLine::M_DrawLine(Controller* controller)
    : controller(controller)
{
}

void M_DrawLine::onMouseClick(QMouseEvent *event, const QVector3D& worldPos) {
    if (event->button() == Qt::LeftButton) {
        if (firstClick) {
            qDebug() << "Draw Line First Phase";
            p1 = controller->addPoint(worldPos.x(), worldPos.y(), 0.0f); //TODO: replace with nearest point
            firstClick = false;
        } else {
            qDebug() << "Draw Line Second Phase";
            p2 = controller->addPoint(worldPos.x(), worldPos.y(), 0.0f);
            controller->addLine(p1, p2);
            firstClick = true;
        }
    } else if (event->button() == Qt::RightButton) {
        if (!firstClick) {
            qDebug() << "Back to first click";
            firstClick = true;
            p1 = nullptr;
        } else {
            qDebug() << "Switch to Selection Mode";
            controller->setModeSelection();
        }
    }
}

