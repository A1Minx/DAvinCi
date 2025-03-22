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

void Orth_XZ_OpenGLWidget::UpdateMatrices() 
{
    float w = width();
    float h = height();

    viewMatrix.setToIdentity();
    viewMatrix.lookAt(
        QVector3D(0, 1, 0), 
        QVector3D(0, 0, 0),  
        QVector3D(0, 0, 1)   
    ); //position camera viewing on xz
    
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

    // draw Grid
    float viewWidth = width() / zoomLevel;
    float viewHeight = height() / zoomLevel;

    // keep a bit larger to prevent weird glitches on the edge of the view
    float left = -viewWidth;
    float right = viewWidth;
    float bottom = -viewHeight;
    float top = viewHeight;

    // rounding TODO: Check if necessary
    left = floor(left / gridSize) * gridSize;
    right = ceil(right / gridSize) * gridSize;
    bottom = floor(bottom / gridSize) * gridSize;
    top = ceil(top / gridSize) * gridSize;

    qDebug() << "left: " << left << "right: " << right << "bottom: " << bottom << "top: " << top;

    // grid data

    std::vector<float> newGridData;
    
    for (float x = left; x <= right; x += gridSize) {
        // position
        newGridData.push_back(x);
        newGridData.push_back(0.0f);
        newGridData.push_back(bottom);
        // color
        newGridData.push_back(0.5f);
        newGridData.push_back(0.5f);
        newGridData.push_back(0.5f);
        newGridData.push_back(1.0f);
        // width
        newGridData.push_back(0.5f);
        // position
        newGridData.push_back(x);
        newGridData.push_back(0.0f);
        newGridData.push_back(top);
        // color
        newGridData.push_back(0.5f);
        newGridData.push_back(0.5f);
        newGridData.push_back(0.5f);
        newGridData.push_back(1.0f);
        // width
        newGridData.push_back(0.5f);
    }

    for (float z = bottom; z <= top; z += gridSize) {
        // position
        newGridData.push_back(left);
        newGridData.push_back(0.0f);
        newGridData.push_back(z);
        // color
        newGridData.push_back(0.5f);
        newGridData.push_back(0.5f);
        newGridData.push_back(0.5f);
        newGridData.push_back(1.0f);
        // width
        newGridData.push_back(0.5f);
        // position
        newGridData.push_back(right);
        newGridData.push_back(0.0f);
        newGridData.push_back(z);
        // color
        newGridData.push_back(0.5f);
        newGridData.push_back(0.5f);
        newGridData.push_back(0.5f);
        newGridData.push_back(1.0f);
        // width
        newGridData.push_back(0.5f);
    }

    // axis data TODO: check if necessary

    // position
    newGridData.push_back(left);
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    // color
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(1.0f);
    // width
    newGridData.push_back(1.0f);

    //position
    newGridData.push_back(right);
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    // color
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(1.0f);
    // width
    newGridData.push_back(1.0f);
    
    //position
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(top);
    // color
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(1.0f);
    // width
    newGridData.push_back(1.0f);

    //position
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(bottom);
    // color
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(1.0f);
    // width
    newGridData.push_back(1.0f);

    //if( newGridData != gridData) {
        gridData = newGridData;
        glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
        glBufferData(GL_ARRAY_BUFFER, gridData.size() * sizeof(float), gridData.data(), GL_DYNAMIC_DRAW);
    //}
}


