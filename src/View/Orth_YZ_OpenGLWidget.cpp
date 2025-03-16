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

void Orth_YZ_OpenGLWidget::updateBuffers() {
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
    
    for (float y = bottom; y <= top; y += gridSize) {
        newGridData.push_back(0.0f);
        newGridData.push_back(y);
        newGridData.push_back(left);
        newGridData.push_back(0.0f);
        newGridData.push_back(y);
        newGridData.push_back(right);
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

    newGridData.push_back(0.0f);
    newGridData.push_back(left);
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(right);
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

