// Orth_XZ_OpenGLWidget.cpp
#include "Orth_XZ_OpenGLWidget.h"
#include <QOpenGLFunctions>
#include <Model.h>
#include <QDebug>
#include <cmath>


DrawingOpenGLWidget::DrawingOpenGLWidget(Model *model, Controller *controller, QWidget *parent)
    : QOpenGLWidget(parent), model(model), controller(controller),
      zoomLevel(1.0f), gridSize(50.0f),
      pointVBO(0), tempPointVBO(0), lineVBO(0), tempLineVBO(0), gridVBO(0)
{
    setFocusPolicy(Qt::StrongFocus);
}

DrawingOpenGLWidget::~DrawingOpenGLWidget() {
    makeCurrent();
    glDeleteBuffers(1, &pointVBO);
    glDeleteBuffers(1, &tempPointVBO);
    glDeleteBuffers(1, &lineVBO);
    glDeleteBuffers(1, &tempLineVBO);
    glDeleteBuffers(1, &gridVBO);
    doneCurrent();
}

void DrawingOpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // Dark Grey Background
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(1.0f);

    setupBuffers();
}

void DrawingOpenGLWidget::setupBuffers() {
    // Initialize VBO Objects
    glGenBuffers(1, &pointVBO);
    glGenBuffers(1, &tempPointVBO);
    glGenBuffers(1, &lineVBO);
    glGenBuffers(1, &tempLineVBO);
    glGenBuffers(1, &gridVBO);
}

void DrawingOpenGLWidget::updateBuffers() {
    //TODO: Check if for loops can be optimized for performance, think about adding temp elements to the VBOs for persistent elements

    // Update points
    std::vector<float> newPointData;
    for (const std::shared_ptr<Point>& point : model->getPoints()) {
        newPointData.push_back(point->getX());
        newPointData.push_back(point->getY());
        newPointData.push_back(point->getZ());
    }
    if (newPointData != pointData) {
        pointData = newPointData;
        glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
        glBufferData(GL_ARRAY_BUFFER, pointData.size() * sizeof(float), pointData.data(), GL_DYNAMIC_DRAW);
    }

    // Update temporary points
    std::vector<float> newTempPointData;
    for (const TempPoint& point : model->getTempPoints()) {
        newTempPointData.push_back(point.x);
        newTempPointData.push_back(point.y);
        newTempPointData.push_back(point.z);
    }
    if (newTempPointData != tempPointData) {
        tempPointData = newTempPointData;
        glBindBuffer(GL_ARRAY_BUFFER, tempPointVBO);
        glBufferData(GL_ARRAY_BUFFER, tempPointData.size() * sizeof(float), tempPointData.data(), GL_DYNAMIC_DRAW);
    }

    // Update lines
    std::vector<float> newLineData;
    for (const std::shared_ptr<Line>& line : model->getLines()) {
        newLineData.push_back(line->getP1()->getX());
        newLineData.push_back(line->getP1()->getY());
        newLineData.push_back(line->getP1()->getZ());
        newLineData.push_back(line->getP2()->getX());
        newLineData.push_back(line->getP2()->getY());
        newLineData.push_back(line->getP2()->getZ());
    }
    if (newLineData != lineData) {
        lineData = newLineData;
        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
        glBufferData(GL_ARRAY_BUFFER, lineData.size() * sizeof(float), lineData.data(), GL_DYNAMIC_DRAW);
    }

    // Update temp lines
    std::vector<float> newTempLineData;
    for (const TempLine& line : model->getTempLines()) {
        newTempLineData.push_back(line.x1);
        newTempLineData.push_back(line.y1);
        newTempLineData.push_back(line.z1);
        newTempLineData.push_back(line.x2);
        newTempLineData.push_back(line.y2);
        newTempLineData.push_back(line.z2);
    }
    if (newTempLineData != tempLineData) {
        tempLineData = newTempLineData;
        glBindBuffer(GL_ARRAY_BUFFER, tempLineVBO);
        glBufferData(GL_ARRAY_BUFFER, tempLineData.size() * sizeof(float), tempLineData.data(), GL_DYNAMIC_DRAW);
    }
}

void DrawingOpenGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    updateProjectionMatrix();
}

void DrawingOpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    updateBuffers();

    glMatrixMode(GL_PROJECTION); // apply following transformations to the projection matrix
    glLoadMatrixf(projectionMatrix.constData()); // load the projection matrix
    glScalef(zoomLevel, zoomLevel, 1.0f); // scale the projection matrix to zoom level
    glMatrixMode(GL_MODELVIEW); // apply following transformations to the vertices

    
    
    drawGrid();

    drawPoints();  
    drawLines();    

    drawTempPoints();       
    drawTempLines();    
}

void DrawingOpenGLWidget::drawGrid() {
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
    
    // Vertikale Linien
    glBegin(GL_LINES);
    for (float x = left; x <= right; x += gridSize) {
        glVertex3f(x, 0.0f, bottom);  
        glVertex3f(x, 0.0f, top);     
    }
    
    for (float z = bottom; z <= top; z += gridSize) {
        glVertex3f(left, 0.0f, z);    
        glVertex3f(right, 0.0f, z);   
    }
    glEnd();
    
    // Zeichne Achsen
    glColor3f(0.5f, 0.5f, 0.5f); // Hellere Farbe für Achsen
    glLineWidth(2.0f);
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

void DrawingOpenGLWidget::drawPoints() {
    if (pointData.empty()) return;

    glColor3f(1.0f, 0.0f, 0.0f);  // Red
    glPointSize(5.0f);
    
    glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);  
    glDrawArrays(GL_POINTS, 0, pointData.size() / 3);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void DrawingOpenGLWidget::drawTempPoints() {
    if (tempPointData.empty()) return;

    glColor3f(0.0f, 1.0f, 0.0f);  // Green
    glPointSize(10.0f);
    
    glBindBuffer(GL_ARRAY_BUFFER, tempPointVBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_POINTS, 0, tempPointData.size() / 3);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void DrawingOpenGLWidget::drawLines() {
    if (lineData.empty()) return;

    glColor3f(1.0f, 0.0f, 0.0f);  // Red
    glLineWidth(2.0f);
    
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_LINES, 0, lineData.size() / 3);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void DrawingOpenGLWidget::drawTempLines() {
    if (tempLineData.empty()) return;

    glColor3f(0.0f, 1.0f, 0.0f);  // Green
    glLineWidth(0.5f);
    
    glBindBuffer(GL_ARRAY_BUFFER, tempLineVBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_LINES, 0, tempLineData.size() / 3);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void DrawingOpenGLWidget::updateProjectionMatrix() {
    float w = width();
    float h = height();
    
    projectionMatrix.setToIdentity();
    projectionMatrix.ortho(-w/2, w/2, -h/2, h/2, -1, 1);
}

void DrawingOpenGLWidget::update() {
    //qDebug() << "updating View";
    QOpenGLWidget::update();
}

QVector3D DrawingOpenGLWidget::screenToWorld(int x, int y) {
    // Konvertiere die Y-Koordinate, da OpenGL den Ursprung unten links hat
    y = height() - y;
    
    // Normalisiere die Koordinaten auf den Bereich [-1, 1]
    float normalizedX = (2.0f * x / width()) - 1.0f;
    float normalizedZ = (2.0f * y / height()) - 1.0f;  
    
    // Skaliere mit dem Zoom-Faktor
    normalizedX /= zoomLevel;
    normalizedZ /= zoomLevel;
    
    // Skaliere mit der Fenstergröße
    float worldX = normalizedX * (width() / 2.0f);
    float worldZ = normalizedZ * (height() / 2.0f);  // Y wird zu Z
    
    return QVector3D(worldX, 0.0f, worldZ);  // Y ist konstant 0, Z enthält die ursprüngliche Y-Koordinate
}


// Control events
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

void DrawingOpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
    controller->handleMouseMove(event);
}

