// Orth_XZ_OpenGLWidget.cpp
#include "Orth_YZ_OpenGLWidget.h"
#include <QOpenGLFunctions>
#include <Model.h>
#include <QDebug>
#include <cmath>

Orth_YZ_OpenGLWidget::Orth_YZ_OpenGLWidget(Model *model, Controller *controller, QWidget *parent)
    : View_OpenGLWidget(model, controller, parent)
{
    cameraPosition = QVector3D(200, 0, 0); //position camera viewing on yz
    cameraTarget = QVector3D(0, 0, 0); //look at origin
    cameraUp = QVector3D(0, 0, 1); //up vector
}

Orth_YZ_OpenGLWidget::~Orth_YZ_OpenGLWidget()
{
}

void Orth_YZ_OpenGLWidget::UpdateMatrices() {
    float w = width();
    float h = height();
    
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(
        cameraPosition, 
        cameraTarget,  
        cameraUp   
    ); //position camera viewing on yz
    
    //move orthogonal camera
    viewMatrix.translate(0, panOffsetX, panOffsetY);
    
    projectionMatrix.setToIdentity();
    projectionMatrix.ortho(-w/(2.0f), w/(2.0f), 
                          -h/(2.0f), h/(2.0f),
                          -1000, 1000);
    projectionMatrix.scale(zoomLevel);
}

QVector3D Orth_YZ_OpenGLWidget::screenToWorld(int x, int y) {
    y = height() - y; // Y-Koordinate umkehren (OpenGL hat Ursprung unten links, Qt oben links)
    
    // Normalisiere die Koordinaten auf den Bereich [-1, 1]
    // Die X-Koordinate des Bildschirms entspricht der Y-Koordinate in der Welt
    // Die Y-Koordinate des Bildschirms entspricht der Z-Koordinate in der Welt
    float normalizedY = (2.0f * x / width()) - 1.0f;
    float normalizedZ = (2.0f * y / height()) - 1.0f;
    
    // Skaliere mit dem Zoom-Faktor
    normalizedY /= zoomLevel;
    normalizedZ /= zoomLevel;
    
    // Skaliere mit der Fenstergröße
    float worldY = normalizedY * (width() / 2.0f);
    float worldZ = normalizedZ * (height() / 2.0f);
    
    // Da wir in der YZ-Ebene sind, ist die X-Koordinate konstant (horizon)
    // Überprüfe ob die Y und Z Koordinaten korrekt zugeordnet sind
    // Möglicherweise musst du hier die Reihenfolge ändern, falls eine Spiegelung auftritt
    return QVector3D(horizon, worldY, worldZ);
}

void Orth_YZ_OpenGLWidget::updateBuffers() {
    View_OpenGLWidget::updateBuffers();
    GridData('x', horizon, gridSize, gridExtend, gridExtend, gridWidth, gridColor);
}

