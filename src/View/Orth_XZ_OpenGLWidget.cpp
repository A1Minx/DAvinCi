// Orth_XZ_OpenGLWidget.cpp
#include "Orth_XZ_OpenGLWidget.h"
#include <QOpenGLFunctions>
#include <Model.h>
#include <QDebug>
#include <cmath>

Orth_XZ_OpenGLWidget::Orth_XZ_OpenGLWidget(Model *model, Controller *controller, QWidget *parent)
    : View_OpenGLWidget(model, controller, parent)
{
}

Orth_XZ_OpenGLWidget::~Orth_XZ_OpenGLWidget()
{
}

void Orth_XZ_OpenGLWidget::drawGrid() {
    //TODO: rework, make grid Size changeable and show world coordinates
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
        glVertex3f(x, 0.0f, bottom);  // X-Z Ebene
        glVertex3f(x, 0.0f, top);     // X-Z Ebene
    }
    // Horizontale Linien (Z-Achse)
    for (float z = bottom; z <= top; z += gridSize) {
        glVertex3f(left, 0.0f, z);    // X-Z Ebene
        glVertex3f(right, 0.0f, z);   // X-Z Ebene
    }
    glEnd();
    
    // Zeichne Achsen
    glColor3f(0.5f, 0.5f, 0.5f); // Hellere Farbe für Achsen
    glLineWidth(2.0f);

    //TODO: Move to VBO
    glBegin(GL_LINES);
    // X-Achse
    glVertex3f(left, 0.0f, 0.0f);
    glVertex3f(right, 0.0f, 0.0f);
    // Z-Achse
    glVertex3f(0.0f, 0.0f, bottom);
    glVertex3f(0.0f, 0.0f, top);
    glEnd();
    glLineWidth(1.0f);
}

void Orth_XZ_OpenGLWidget::updateProjectionMatrix() {
    float w = width();
    float h = height();
    
    projectionMatrix.setToIdentity();
    projectionMatrix.ortho(-w/(2.0f*zoomLevel), w/(2.0f*zoomLevel), 
                          -1000, 1000,
                          -h/(2.0f*zoomLevel), h/(2.0f*zoomLevel));
    projectionMatrix.scale(zoomLevel, zoomLevel, 1.0f);
}

QVector3D Orth_XZ_OpenGLWidget::screenToWorld(int x, int z) {
    // Konvertiere die z-Koordinate, da OpenGL den Ursprung unten links hat
    z = height() - z;
    
    // Normalisiere die Koordinaten auf den Bereich [-1, 1]
    float normalizedX = (2.0f * x / width()) - 1.0f;
    float normalizedZ = (2.0f * z / height()) - 1.0f;
    
    // Skaliere mit dem Zoom-Faktor
    normalizedX /= zoomLevel;
    normalizedZ /= zoomLevel;
    
    // Skaliere mit der Fenstergröße
    float worldX = normalizedX * (width() / 2.0f);
    float worldZ = normalizedZ * (height() / 2.0f);
    
    return QVector3D(worldX, 0.0f, worldZ);
}


