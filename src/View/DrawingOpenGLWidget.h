#ifndef DRAWINGOPENGLWIDGET_H
#define DRAWINGOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QVector3D>
#include <Model.h>
#include <Controller.h>

class Controller;

class DrawingOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit DrawingOpenGLWidget(Model *model, Controller *controller, QWidget *parent = nullptr);
    ~DrawingOpenGLWidget();

    void update(); 

    // Konvertiert Bildschirmkoordinaten in Weltkoordinaten
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
    void drawGrid();
    void drawModel();
    void drawTemporary();
    void updateProjectionMatrix();

    Model *model;
    Controller *controller;

    // View parameters
    QMatrix4x4 projectionMatrix;
    float zoomLevel;        // Zoom-Faktor
    float gridSize;         // Größe des Gitters
};

#endif // DRAWINGOPENGLWIDGET_H
