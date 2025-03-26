#include "View_OpenGLWidget.h"
#include "Controller.h"
#include <QKeyEvent>

void View_OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton) {
        isPanning = true;
        lastPanPosition = event->pos();
        setCursor(Qt::ClosedHandCursor);
    } else if (controller) {
        controller->handleMouseClick(event);
    }
}

void View_OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (isPanning) {
        QPoint delta = event->pos() - lastPanPosition;
        
        float sensitivity = 1.0f / zoomLevel;
        panOffsetX += delta.x() * sensitivity;
        panOffsetY -= delta.y() * sensitivity;
        
        lastPanPosition = event->pos();
        
        UpdateMatrices();
        update();
    } else if (controller) {
        controller->handleMouseMove(event);
    }
}

void View_OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton) {
        isPanning = false;
        setCursor(Qt::ArrowCursor);
    }
}

void View_OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control) {
        ctrlPressed = true;
    }
}

void View_OpenGLWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control) {
        ctrlPressed = false;
    }
} 