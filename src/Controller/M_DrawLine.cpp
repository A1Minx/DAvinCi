#include "M_DrawLine.h"
#include "Controller.h"
#include "Point.h"
#include <QDebug>

M_DrawLine::M_DrawLine(Controller* controller)
    : controller(controller)
{
}

// void M_DrawLine::onMouseClickNewPoints(QMouseEvent *event, const QVector3D& worldPos) {
//     //Deprecated function, keeping it for documentation / future need
//         if (event->button() == Qt::LeftButton) {
//         if (firstClick) {
//             qDebug() << "Draw Line First Phase";
//             p1 = controller->addPoint(worldPos.x(), worldPos.y(), 0.0f); //TODO: replace with nearest point
//             firstClick = false;
//         } else {
//             qDebug() << "Draw Line Second Phase";
//             p2 = controller->addPoint(worldPos.x(), worldPos.y(), 0.0f);
//             controller->addLine(p1, p2);
//             firstClick = true;
//             controller->removeTempLines();
//         }
//     } else if (event->button() == Qt::RightButton) {
//         if (!firstClick) {
//             qDebug() << "Back to first click";
//             firstClick = true;
//             p1 = nullptr;
//         } else {
//             qDebug() << "Switch to Selection Mode";
//             controller->removeTempLines();
//             controller->setModeSelection();
//         }
//     }
// }

void M_DrawLine::onMouseClick(QMouseEvent *event, const QVector3D& worldPos) {
    //TODO: Change to switch, try out bitmasking the states
    if (nearestPoint) {
        if (event->button() == Qt::LeftButton) {
            if (firstClick) {
                qDebug() << "Draw Line First Phase";
                p1 = nearestPoint;
                firstClick = false;
            } else {
                qDebug() << "Draw Line Second Phase";
                p2 = nearestPoint;
                controller->addLine(p1, p2);
                firstClick = true;
                controller->removeTempLines();
            }
        } else if (event->button() == Qt::RightButton) {
            if (!firstClick) {
                qDebug() << "Back to first click";
                firstClick = true;
                p1 = nullptr;
                controller->removeTempLines();
            } else {
                qDebug() << "Switch to Selection Mode";
                controller->removeTempLines();
                controller->setModeSelection();
            }
        }
    } else {
        if (event->button() == Qt::LeftButton) {
            qDebug() << "no nearest point found";
        } else if (event->button() == Qt::RightButton) {
            if (!firstClick) {
                qDebug() << "Back to first click";
                firstClick = true;
                p1 = nullptr;
                controller->removeTempLines();
            } else {
                qDebug() << "Switch to Selection Mode";
                controller->removeTempLines();
                controller->setModeSelection();
            }
        }
    }
}

void M_DrawLine::onMouseMove(QMouseEvent *event, const QVector3D& worldPos) {
    
    nearestPoint = controller->getNearestPoint(worldPos.x(), worldPos.y(), worldPos.z());
    if (nearestPoint) {
        controller->removeTempPoints();
        controller->addTempPoint(nearestPoint->getX(), nearestPoint->getY(), nearestPoint->getZ());
    } else {
        controller->removeTempPoints();
    }

    if (!firstClick) {
        controller->removeTempLines();
        controller->addTempLine(p1->getX(), p1->getY(), p1->getZ(), worldPos.x(), worldPos.y(), worldPos.z());
    } 
}

