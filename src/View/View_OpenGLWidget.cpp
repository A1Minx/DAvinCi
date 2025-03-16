#include "View_OpenGLWidget.h"
#include "Controller.h"

void View_OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    if (controller) {
        controller->handleMouseClick(event);
    }
}

void View_OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (controller) {
        controller->handleMouseMove(event);
    }
} 