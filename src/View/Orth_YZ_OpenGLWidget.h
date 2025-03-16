#ifndef ORTH_YZ_OPENGLWIDGET_H
#define ORTH_YZ_OPENGLWIDGET_H

#include "View_OpenGLWidget.h"

class Orth_YZ_OpenGLWidget : public View_OpenGLWidget {
    Q_OBJECT

public:
    explicit Orth_YZ_OpenGLWidget(Model *model, Controller *controller, QWidget *parent = nullptr);
    ~Orth_YZ_OpenGLWidget() override;

    QVector3D screenToWorld(int x, int y) override;

protected:
    void drawGrid() override;
    void updateProjectionMatrix() override;
};

#endif // ORTH_YZ_OPENGLWIDGET_H
