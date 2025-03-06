// DrawingOpenGLWidget.cpp
#include "DrawingOpenGLWidget.h"
#include <QOpenGLFunctions>
#include <Model.h>
#include <QDebug>
#include <cmath>

DrawingOpenGLWidget::DrawingOpenGLWidget(Model *model, Controller *controller, QWidget *parent)
    : QOpenGLWidget(parent), model(model), controller(controller),
      cameraDistance(5.0f), cameraRotationX(0.0f), cameraRotationY(0.0f),
      isRotating(false)
{
    setFocusPolicy(Qt::StrongFocus);
}

DrawingOpenGLWidget::~DrawingOpenGLWidget() {
}

void DrawingOpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // Dark gray background
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(5.0f);
}

void DrawingOpenGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45.0f, float(w) / float(h), 0.1f, 100.0f);
}

void DrawingOpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    updateCamera();
    
    // Apply camera transformations
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projectionMatrix.constData());
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(viewMatrix.constData());
    
    drawPoints();
}

void DrawingOpenGLWidget::updateCamera() {
    viewMatrix.setToIdentity();
    
    // Apply rotations
    viewMatrix.rotate(cameraRotationX, 1.0f, 0.0f, 0.0f);
    viewMatrix.rotate(cameraRotationY, 0.0f, 1.0f, 0.0f);
    
    // Move camera back
    viewMatrix.translate(0.0f, 0.0f, -cameraDistance);
}

void DrawingOpenGLWidget::drawPoints() {
    const auto& points = model->getPoints();
    
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f); // White points
    
    for (const auto& point : points) {
        glVertex3f(point->x, point->y, point->z);
    }
    
    glEnd();
}

void DrawingOpenGLWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        controller->handleMouseClick(event);
    } else if (event->button() == Qt::RightButton) {
        isRotating = true;
        lastMousePos = event->pos();
    }
}

void DrawingOpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
    if (isRotating) {
        int dx = event->x() - lastMousePos.x();
        int dy = event->y() - lastMousePos.y();
        
        cameraRotationY += dx * 0.5f;
        cameraRotationX += dy * 0.5f;
        
        // Limit vertical rotation
        cameraRotationX = std::max(-90.0f, std::min(90.0f, cameraRotationX));
        
        lastMousePos = event->pos();
        update();
    }
}

void DrawingOpenGLWidget::wheelEvent(QWheelEvent *event) {
    float delta = event->angleDelta().y() / 120.0f;
    cameraDistance = std::max(1.0f, std::min(20.0f, cameraDistance - delta));
    update();
}

void DrawingOpenGLWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        isRotating = false;
    }
}
