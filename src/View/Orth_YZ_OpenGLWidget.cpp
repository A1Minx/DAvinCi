// Orth_XZ_OpenGLWidget.cpp
#include "Orth_YZ_OpenGLWidget.h"
#include <QOpenGLFunctions>
#include <Model.h>
#include <QDebug>
#include <cmath>

Orth_YZ_OpenGLWidget::Orth_YZ_OpenGLWidget(Model *model, Controller *controller, QWidget *parent)
    : View_OpenGLWidget(model, controller, parent)
{
}

Orth_YZ_OpenGLWidget::~Orth_YZ_OpenGLWidget()
{
}

void Orth_YZ_OpenGLWidget::drawGrid() {
    float viewWidth = width() / zoomLevel;
    float viewHeight = height() / zoomLevel;
    
    // visible area
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
    
    // Vertikale Linien (X-Achse)
    glBegin(GL_LINES);
    for (float x = left; x <= right; x += gridSize) {
        glVertex3f(x, 100.0f, bottom);  // X-Y Ebene
        glVertex3f(x, 100.0f, top);     // X-Y Ebene
    }
    // Horizontale Linien (Y-Achse)
    for (float y = bottom; y <= top; y += gridSize) {
        glVertex3f(left, y, 100.0f);    // X-Y Ebene
        glVertex3f(right, y, 100.0f);   // X-Y Ebene
    }
    glEnd();
    
    // Zeichne Achsen
    glColor3f(0.5f, 0.5f, 0.5f); // Hellere Farbe für Achsen
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    // X-Achse
    glVertex3f(left, 0.0f, 0.0f);
    glVertex3f(right, 0.0f, 0.0f);
    // Y-Achse
    glVertex3f(0.0f, bottom, 0.0f);
    glVertex3f(0.0f, top, 0.0f);
    glEnd();
    glLineWidth(1.0f);
}

void Orth_YZ_OpenGLWidget::updateProjectionMatrix() {
    float w = width();
    float h = height();
    
    
    projectionMatrix.setToIdentity();
    projectionMatrix.ortho(-1000, 1000,
                          -w/(2.0f*zoomLevel), w/(2.0f*zoomLevel), 
                          -h/(2.0f*zoomLevel), h/(2.0f*zoomLevel));
    projectionMatrix.scale(zoomLevel, zoomLevel, 1.0f);
}

QVector3D Orth_YZ_OpenGLWidget::screenToWorld(int x, int y) {
    y = height() - y;
    
    // Normalisiere die Koordinaten auf den Bereich [-1, 1]
    float normalizedY = (2.0f * x / width()) - 1.0f;
    float normalizedZ = (2.0f * y / height()) - 1.0f;
    
    // Skaliere mit dem Zoom-Faktor
    normalizedY /= zoomLevel;
    normalizedZ /= zoomLevel;
    
    // Skaliere mit der Fenstergröße
    float worldY = normalizedY * (width() / 2.0f);
    float worldZ = normalizedZ * (height() / 2.0f);
    
    return QVector3D(0.0f, worldY, worldZ);
}

