#include "M_DrawLine.h"
#include "View/DrawingOpenGLWidget.h"
#include "Controller.h"
#include <QDebug>

M_DrawLine::M_DrawLine(Controller* controller, DrawingOpenGLWidget* view)
    : controller(controller), view(view)
{

}



void M_DrawLine::onMouseClick(QMouseEvent *event) {

    // Normalize mouse coordinates to OpenGL (-1 to 1 range)
    float x = 2.0f * event->x() / view->width() - 1.0f;
    float y = 1.0f - 2.0f * event->y() / view->height();

    if (firstClick) {
        qDebug() << "Draw Line First Phase";
        x1 = x;
        y1 = y;
        firstClick = false;
    } else {
        qDebug() << "Draw Line Second Phase";
        controller->addLine(x1, y1, x, y);
        view->update();
        firstClick = true;
    }
}

