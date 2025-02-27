#ifndef DRAWINGOPENGLWIDGET_H
#define DRAWINGOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <Model.h>
#include <ModeController.h>

class DrawingOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit DrawingOpenGLWidget(Model *model, QWidget *parent = nullptr);
    void configModeController(ModeController *modeController);
    ~DrawingOpenGLWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
private:
    Model *model;
    ModeController *modeController;
};

#endif // DRAWINGOPENGLWIDGET_H
