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

void Orth_XZ_OpenGLWidget::updateBuffers() {
    View_OpenGLWidget::updateBuffers();

    // draw Grid
    float viewWidth = width() / zoomLevel;
    float viewHeight = height() / zoomLevel;

    float left = -viewWidth/2;
    float right = viewWidth/2;
    float bottom = -viewHeight/2;
    float top = viewHeight/2;

    // rounding TODO: Check if necessary
    left = floor(left / gridSize) * gridSize;
    right = ceil(right / gridSize) * gridSize;
    bottom = floor(bottom / gridSize) * gridSize;
    top = ceil(top / gridSize) * gridSize;

    qDebug() << "left: " << left << "right: " << right << "bottom: " << bottom << "top: " << top;

    // grid data
    glColor4f(0.3f, 0.3f, 0.3f, 0.5f); // light grey

    std::vector<float> newGridData;
    
    for (float x = left; x <= right; x += gridSize) {
        newGridData.push_back(x);
        newGridData.push_back(0.0f);
        newGridData.push_back(bottom);
        newGridData.push_back(x);
        newGridData.push_back(0.0f);
        newGridData.push_back(top);
    }

    for (float z = bottom; z <= top; z += gridSize) {
        newGridData.push_back(left);
        newGridData.push_back(0.0f);
        newGridData.push_back(z);
        newGridData.push_back(right);
        newGridData.push_back(0.0f);
        newGridData.push_back(z);
    }

    // axis data TODO: check if necessary
    
    glColor3f(0.5f, 0.5f, 0.5f);
    glLineWidth(2.0f);

    newGridData.push_back(left);
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(right);
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);

    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(bottom);
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(top);


    //if( newGridData != gridData) {
        gridData = newGridData;
        glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
        glBufferData(GL_ARRAY_BUFFER, gridData.size() * sizeof(float), gridData.data(), GL_DYNAMIC_DRAW);
    //}
}


