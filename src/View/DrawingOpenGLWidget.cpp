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
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw red line
    glColor3f(1.0f, 0.0f, 0.0f); // Red color

    glBegin(GL_LINES);
    for (const auto &line : model->getLines()) {
        glVertex2f(line.first.first, line.first.second);   // Start point
        glVertex2f(line.second.first, line.second.second); // End point
    }
    glEnd();

    glFlush();
}

void DrawingOpenGLWidget::mousePressEvent(QMouseEvent *event) {

    qDebug() << "MouseClicked";
    qDebug() << modeController->getCurrentMode();
    modeController->getCurrentMode()->onMouseClick(event);

}
