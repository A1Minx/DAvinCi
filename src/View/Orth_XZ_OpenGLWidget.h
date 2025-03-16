#ifndef ORTH_XZ_OPENGLWIDGET_H
#define ORTH_XZ_OPENGLWIDGET_H

#include "View_OpenGLWidget.h"

class Orth_XZ_OpenGLWidget : public View_OpenGLWidget {
    Q_OBJECT

public:
    explicit Orth_XZ_OpenGLWidget(Model *model, Controller *controller, QWidget *parent = nullptr);
    ~Orth_XZ_OpenGLWidget() override;

    QVector3D screenToWorld(int x, int y) override;

protected:
    void drawGrid() override;
    void updateProjectionMatrix() override;
};

#endif // ORTH_XZ_OPENGLWIDGET_H
