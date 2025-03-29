// Orth_XY_OpenGLWidget.cpp
#include "Orth_XY_OpenGLWidget.h"
#include <QOpenGLFunctions>
#include <Model.h>
#include <QDebug>
#include <cmath>

Orth_XY_OpenGLWidget::Orth_XY_OpenGLWidget(Model *model, Controller *controller, QWidget *parent)
    : View_OpenGLWidget(model, controller, parent)
{
    cameraPosition = QVector3D(0, 0, 1);
    cameraTarget = QVector3D(0, 0, 0);
    cameraUp = QVector3D(0, 1, 0);
}

Orth_XY_OpenGLWidget::~Orth_XY_OpenGLWidget()
{
}

void Orth_XY_OpenGLWidget::UpdateMatrices() {
    float w = width();
    float h = height();

    viewMatrix.setToIdentity();
    viewMatrix.lookAt(
        QVector3D(0, 0, 200),  //position camera viewing on xy
        QVector3D(0, 0, 0),  //look at origin
        QVector3D(0, 1, 0)   //up vector
    );
    
    //move orthogonal camera
    viewMatrix.translate(panOffsetX, panOffsetY, 0);
    
    projectionMatrix.setToIdentity();
    projectionMatrix.ortho(-w/(2.0f), w/(2.0f), 
                          -h/(2.0f), h/(2.0f), 
                          -1000, 1000);

    projectionMatrix.scale(zoomLevel);
}

QVector3D Orth_XY_OpenGLWidget::screenToWorld(int x, int y) {
    y = height() - y;
    
    float normalizedX = (2.0f * x / width()) - 1.0f;
    float normalizedY = (2.0f * y / height()) - 1.0f;
    
    normalizedX /= zoomLevel;
    normalizedY /= zoomLevel;
    
    float worldX = normalizedX * (width() / 2.0f);
    float worldY = normalizedY * (height() / 2.0f);
    
    return QVector3D(worldX, worldY, horizon);
}

void Orth_XY_OpenGLWidget::updateBuffers() {
    View_OpenGLWidget::updateBuffers();

    GridData('z', horizon, gridSize, gridExtend, gridExtend, gridWidth, gridColor);
}

