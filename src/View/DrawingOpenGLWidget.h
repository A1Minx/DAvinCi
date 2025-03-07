#ifndef DRAWINGOPENGLWIDGET_H
#define DRAWINGOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <Model.h>
#include <Controller.h>

class Controller;

class DrawingOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit DrawingOpenGLWidget(Model *model, Controller *controller, QWidget *parent = nullptr);
    ~DrawingOpenGLWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void updateCamera();
    void drawCube();

    Model *model;
    Controller *controller;

    // Camera parameters
    QMatrix4x4 projectionMatrix;
    QMatrix4x4 viewMatrix;
    float cameraDistance;
    float cameraRotationX;
    float cameraRotationY;
    QPoint lastMousePos;
};

#endif // DRAWINGOPENGLWIDGET_H
