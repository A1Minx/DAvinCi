// DrawingOpenGLWidget.cpp
#include "DrawingOpenGLWidget.h"
#include <QOpenGLFunctions>
#include <Model.h>
#include <QDebug>
#include <cmath>

DrawingOpenGLWidget::DrawingOpenGLWidget(Model *model, Controller *controller, QWidget *parent)
    : QOpenGLWidget(parent), model(model), controller(controller),
      cameraDistance(5.0f), cameraRotationX(0.0f), cameraRotationY(0.0f)
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
    glPointSize(10.0f);
    cameraDistance = 500.0f; // Größere Distanz für bessere Übersicht
    qDebug() << "OpenGL initialisiert";
}

void DrawingOpenGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45.0f, float(w) / float(h), 0.1f, 1000.0f); // Größerer Far-Clip
    qDebug() << "Viewport resized:" << w << "x" << h;
}

void DrawingOpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    updateCamera();
    
    // Apply camera transformations
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projectionMatrix.constData());
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(viewMatrix.constData());
    
    drawCube();
}

void DrawingOpenGLWidget::updateCamera() {
    viewMatrix.setToIdentity();
    
    // Apply rotations
    viewMatrix.rotate(cameraRotationX, 1.0f, 0.0f, 0.0f);
    viewMatrix.rotate(cameraRotationY, 0.0f, 1.0f, 0.0f);
    
    // Move camera back
    viewMatrix.translate(0.0f, 0.0f, -cameraDistance);
}

void DrawingOpenGLWidget::drawCube() {
    float size = 200.0f; // 200x200x200 Würfel
    float halfSize = size / 2.0f;
    
    qDebug() << "Zeichne Würfel mit Größe:" << size;
    
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f); // Rot
    
    // Vorderseite
    glVertex3f(-halfSize, -halfSize, halfSize);
    glVertex3f(halfSize, -halfSize, halfSize);
    glVertex3f(halfSize, halfSize, halfSize);
    glVertex3f(-halfSize, halfSize, halfSize);
    
    // Rückseite
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f(-halfSize, halfSize, -halfSize);
    glVertex3f(halfSize, halfSize, -halfSize);
    glVertex3f(halfSize, -halfSize, -halfSize);
    
    // Oberseite
    glVertex3f(-halfSize, halfSize, -halfSize);
    glVertex3f(-halfSize, halfSize, halfSize);
    glVertex3f(halfSize, halfSize, halfSize);
    glVertex3f(halfSize, halfSize, -halfSize);
    
    // Unterseite
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f(halfSize, -halfSize, -halfSize);
    glVertex3f(halfSize, -halfSize, halfSize);
    glVertex3f(-halfSize, -halfSize, halfSize);
    
    // Rechte Seite
    glVertex3f(halfSize, -halfSize, -halfSize);
    glVertex3f(halfSize, halfSize, -halfSize);
    glVertex3f(halfSize, halfSize, halfSize);
    glVertex3f(halfSize, -halfSize, halfSize);
    
    // Linke Seite
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f(-halfSize, -halfSize, halfSize);
    glVertex3f(-halfSize, halfSize, halfSize);
    glVertex3f(-halfSize, halfSize, -halfSize);
    
    glEnd();
    qDebug() << "Würfel gezeichnet";
}

void DrawingOpenGLWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        controller->handleMouseClick(event);
    }
    lastMousePos = event->pos();
    grabMouse(); // Fängt die Maus ein, damit wir auch außerhalb des Widgets Mausbewegungen erhalten
}

void DrawingOpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
    int dx = event->x() - lastMousePos.x();
    int dy = event->y() - lastMousePos.y();
    
    cameraRotationY += dx * 0.5f;
    cameraRotationX += dy * 0.5f;
    
    // Limit vertical rotation
    cameraRotationX = std::max(-90.0f, std::min(90.0f, cameraRotationX));
    
    lastMousePos = event->pos();
    update();
}

void DrawingOpenGLWidget::wheelEvent(QWheelEvent *event) {
    float delta = event->angleDelta().y() / 120.0f;
    cameraDistance = std::max(100.0f, std::min(1000.0f, cameraDistance - delta));
    qDebug() << "Kamera-Distanz:" << cameraDistance;
    update();
}

void DrawingOpenGLWidget::mouseReleaseEvent(QMouseEvent *event) {
    releaseMouse(); // Gibt die Maus wieder frei
}

