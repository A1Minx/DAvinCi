#ifndef ORTH_XY_OPENGLWIDGET_H
#define ORTH_XY_OPENGLWIDGET_H

#include "View_OpenGLWidget.h"

class Orth_XY_OpenGLWidget : public View_OpenGLWidget {
    Q_OBJECT

public:
    explicit Orth_XY_OpenGLWidget(Model *model, Controller *controller, QWidget *parent = nullptr);
    ~Orth_XY_OpenGLWidget() override;

    QVector3D screenToWorld(int x, int y) override;

protected:
    void updateProjectionMatrix() override;
    void updateBuffers() override;
};

#endif // ORTH_XY_OPENGLWIDGET_H
