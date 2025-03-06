#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <iostream>
#include <DrawingOpenGLWidget.h>
#include <ModeController.h>



class Controller : public QObject
{
    Q_OBJECT

public:
    Controller(Model *model, DrawingOpenGLWidget *view);
    void setView(DrawingOpenGLWidget *view);

    ModeController* getModeController();

public slots:
    void save();
    void load();
    void clearShapes();
    void setModeDrawPoint();
    void setModeDrawLine();
    void writeSQL();
    void readSQL();
    void handleMouseClick(QMouseEvent *event);

public:
    Model* getModel();
    void addLine(float x1, float y1, float x2, float y2);
    void addShape();
    void addPoint(float x, float y, float z);

private:
    Model *model;
    DrawingOpenGLWidget *view;
    ModeController *modeController;
};

#endif
