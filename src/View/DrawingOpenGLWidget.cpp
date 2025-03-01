// DrawingOpenGLWidget.cpp
#include "DrawingOpenGLWidget.h"
#include <QOpenGLFunctions>
#include <Model.h>
#include <QDebug>

DrawingOpenGLWidget::DrawingOpenGLWidget(Model *model, QWidget *parent)
    : QOpenGLWidget(parent), model(model) {
}

void DrawingOpenGLWidget::configModeController(ModeController *modeController)
{
    qDebug() << "ModeController for View configured";
    this->modeController = modeController;
}



DrawingOpenGLWidget::~DrawingOpenGLWidget() {
}

void DrawingOpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();



    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // White background
}

void DrawingOpenGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void DrawingOpenGLWidget::paintGL() {


    // Example: Drawing a simple cube


    glEnd();
    //view experiment

    glFlush();
}





void DrawingOpenGLWidget::mousePressEvent(QMouseEvent *event) {
    qDebug() << "MouseClicked";
    qDebug() << modeController->getCurrentMode();
    modeController->getCurrentMode()->onMouseClick(event);
}
