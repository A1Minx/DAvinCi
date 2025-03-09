// DrawingOpenGLWidget.cpp
#include "DrawingOpenGLWidget.h"
#include <QOpenGLFunctions>
#include <Model.h>
#include <QDebug>
#include <cmath>

DrawingOpenGLWidget::DrawingOpenGLWidget(Model *model, Controller *controller, QWidget *parent)
    : QOpenGLWidget(parent), model(model), controller(controller),
      zoomLevel(1.0f), gridSize(50.0f)
{
    setFocusPolicy(Qt::StrongFocus);
}

DrawingOpenGLWidget::~DrawingOpenGLWidget() {
}

void DrawingOpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // Dunkelgrauer Hintergrund
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(1.0f);
}

void DrawingOpenGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    updateProjectionMatrix();
}

void DrawingOpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projectionMatrix.constData());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Nur Zoom anwenden
    glScalef(zoomLevel, zoomLevel, 1.0f);
    
    // Zeichne das Gitter
    drawGrid();
    
    // Zeichne die Punkte
    drawPoints();
}

void DrawingOpenGLWidget::drawGrid() {
    float viewWidth = width() / zoomLevel;
    float viewHeight = height() / zoomLevel;
    
    // Berechne sichtbaren Bereich
    float left = -viewWidth/2;
    float right = viewWidth/2;
    float bottom = -viewHeight/2;
    float top = viewHeight/2;
    
    // Runde zu Gitterlinien
    left = floor(left / gridSize) * gridSize;
    right = ceil(right / gridSize) * gridSize;
    bottom = floor(bottom / gridSize) * gridSize;
    top = ceil(top / gridSize) * gridSize;
    
    glColor4f(0.3f, 0.3f, 0.3f, 0.5f); // Hellgraue Gitterlinien
    
    // Vertikale Linien
    glBegin(GL_LINES);
    for (float x = left; x <= right; x += gridSize) {
        glVertex2f(x, bottom);
        glVertex2f(x, top);
    }
    // Horizontale Linien
    for (float y = bottom; y <= top; y += gridSize) {
        glVertex2f(left, y);
        glVertex2f(right, y);
    }
    glEnd();
    
    // Zeichne Achsen
    glColor3f(0.5f, 0.5f, 0.5f); // Hellere Farbe für Achsen
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    // X-Achse
    glVertex2f(left, 0);
    glVertex2f(right, 0);
    // Y-Achse
    glVertex2f(0, bottom);
    glVertex2f(0, top);
    glEnd();
    glLineWidth(1.0f);
}

void DrawingOpenGLWidget::drawPoints() {
    // Hole die Punkte als shared_ptr
    std::vector<std::shared_ptr<Point>> points = model->getPoints();
    
    if (points.empty()) {
        return;
    }
    
    glColor3f(1.0f, 0.0f, 0.0f); // Rot
    glPointSize(10.0f);
    
    glBegin(GL_POINTS);
    for (const auto& point : points) {
        // Verwende den Point über den shared_ptr
        glVertex3f(point->getX(), point->getY(), point->getZ());
    }
    glEnd();
}

void DrawingOpenGLWidget::mousePressEvent(QMouseEvent *event) {
    controller->handleMouseClick(event);
}

void DrawingOpenGLWidget::wheelEvent(QWheelEvent *event) {
    // Zoom-Faktor anpassen
    float delta = event->angleDelta().y() / 120.0f;
    zoomLevel *= (1.0f + delta * 0.1f);
    zoomLevel = std::max(0.1f, std::min(10.0f, zoomLevel));
    
    updateProjectionMatrix();
    update();
}

void DrawingOpenGLWidget::updateProjectionMatrix() {
    float w = width();
    float h = height();
    
    projectionMatrix.setToIdentity();
    projectionMatrix.ortho(-w/2, w/2, -h/2, h/2, -1, 1);
}

void DrawingOpenGLWidget::update() {
    qDebug() << "updating View";
    QOpenGLWidget::update();
}

QVector3D DrawingOpenGLWidget::screenToWorld(int x, int y) {
    // Konvertiere die Y-Koordinate, da OpenGL den Ursprung unten links hat
    y = height() - y;
    
    // Normalisiere die Koordinaten auf den Bereich [-1, 1]
    float normalizedX = (2.0f * x / width()) - 1.0f;
    float normalizedY = (2.0f * y / height()) - 1.0f;
    
    // Skaliere mit dem Zoom-Faktor
    normalizedX /= zoomLevel;
    normalizedY /= zoomLevel;
    
    // Skaliere mit der Fenstergröße
    float worldX = normalizedX * (width() / 2.0f);
    float worldY = normalizedY * (height() / 2.0f);
    
    qDebug() << "Screen coords:" << x << y << "-> World coords:" << worldX << worldY;
    
    return QVector3D(worldX, worldY, 0.0f);
}

