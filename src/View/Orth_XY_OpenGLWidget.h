#ifndef ORTH_XY_OPENGLWIDGET_H
#define ORTH_XY_OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QVector3D>
#include <Model.h>
#include <Controller.h>

class Controller;

class Orth_XY_OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit Orth_XY_OpenGLWidget(Model *model, Controller *controller, QWidget *parent = nullptr);
    ~Orth_XY_OpenGLWidget();

    void update(); 

    QVector3D screenToWorld(int x, int y);

protected:
    // GL functions
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    // Control events
    void mousePressEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void setupBuffers();
    void updateBuffers();
    void drawGrid();
    void drawPoints();
    void drawLines();
    void drawTempPoints();
    void drawTempLines();
    void updateProjectionMatrix();

    Model *model;
    Controller *controller;
    
    QMatrix4x4 projectionMatrix;
    float zoomLevel;        
    float gridSize;        

    // Buffer Objects
    GLuint pointVBO;
    GLuint tempPointVBO;

    GLuint lineVBO;
    GLuint tempLineVBO;

    GLuint gridVBO;

    // Buffer Data
    std::vector<float> pointData;
    std::vector<float> tempPointData;

    std::vector<float> lineData;
    std::vector<float> tempLineData;

    std::vector<float> gridData;
};

#endif // ORTH_XY_OPENGLWIDGET_H
