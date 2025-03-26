#ifndef FREE_OPENGLWIDGET_H
#define FREE_OPENGLWIDGET_H

#include "View_OpenGLWidget.h"

class Free_OpenGLWidget : public View_OpenGLWidget {
    Q_OBJECT

public:
    explicit Free_OpenGLWidget(Model *model, Controller *controller, QWidget *parent = nullptr);
    ~Free_OpenGLWidget();

    void UpdateMatrices() override;
    QVector3D screenToWorld(int x, int y) override;

private:
    void updateBuffers() override;

    float yaw;      // Camera rotation around y-axis (in degrees)
    float pitch;    // Camera rotation around x-axis (in degrees)
    QVector3D cameraPosition;
    QVector3D cameraTarget;
    QVector3D cameraUp;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    bool isTilting;
};

#endif // FREE_OPENGLWIDGET_H
