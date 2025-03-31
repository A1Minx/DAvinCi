// Orth_XZ_OpenGLWidget.cpp
#include "Orth_XZ_OpenGLWidget.h"
#include <QOpenGLFunctions>
#include <Model.h>
#include <QDebug>
#include <cmath>

Orth_XZ_OpenGLWidget::Orth_XZ_OpenGLWidget(Model *model, Controller *controller, QWidget *parent)
    : View_OpenGLWidget(model, controller, parent)
{
    cameraPosition = QVector3D(0, -200, 0); //position camera viewing on xz
    cameraTarget = QVector3D(0, 0, 0); //look at origin
    cameraUp = QVector3D(0, 0, 1); //up vector
}

Orth_XZ_OpenGLWidget::~Orth_XZ_OpenGLWidget()
{
}

void Orth_XZ_OpenGLWidget::UpdateMatrices() 
{
    float w = width();
    float h = height();

    viewMatrix.setToIdentity();
    viewMatrix.lookAt(
        cameraPosition, 
        cameraTarget,  
        cameraUp   
    ); //position camera viewing on xz
    
    //move orthogonal camera
    viewMatrix.translate(panOffsetX, 0, panOffsetY);
    
    projectionMatrix.setToIdentity();
    projectionMatrix.ortho(-w/(2.0f), w/(2.0f), 
                          -h/(2.0f), h/(2.0f),
                          -1000, 1000);
                          
    projectionMatrix.scale(zoomLevel);
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
    
    return QVector3D(worldX, horizon, worldZ);
}

void Orth_XZ_OpenGLWidget::updateBuffers() {
    View_OpenGLWidget::updateBuffers();

    GridData('y', horizon, gridSize, gridExtend, gridExtend, gridWidth, gridColor);
}


