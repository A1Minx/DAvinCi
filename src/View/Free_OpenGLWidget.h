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

signals:
    void horizonChanged(float value);

private:
    void updateBuffers() override;

    float yaw;      // Camera rotation around y-axis (in degrees)
    float pitch;    // Camera rotation around x-axis (in degrees)

    float cameraDistance = 200.0f; // Fixed camera distance

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override; // Override to disable zooming
    bool isTilting;
};

#endif // FREE_OPENGLWIDGET_H
