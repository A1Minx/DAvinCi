#include "M_DrawPoint.h"
#include "View/DrawingOpenGLWidget.h"
#include "Controller.h"
#include <QDebug>

M_DrawPoint::M_DrawPoint(Controller* controller, DrawingOpenGLWidget* view)
    : controller(controller), view(view)
{

}



void M_DrawPoint::onMouseClick(QMouseEvent *event) {

    // Normalize mouse coordinates to OpenGL (-1 to 1 range)
    float x = 2.0f * event->x() / view->width() - 1.0f;
    float y = 1.0f - 2.0f * event->y() / view->height();



    qDebug() << "Draw Point";
    controller->addLine(x-5, y-5, x+5, y+5);
    controller->addLine(x-5, y+5, x+5, y-5);
    view->update();
}

