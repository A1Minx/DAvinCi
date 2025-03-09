#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <iostream>
#include <DrawingOpenGLWidget.h>
#include <ModeController.h>
#include <QMouseEvent>

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
    void setModeSelection();
    void writeSQL();
    void readSQL();
    void handleMouseClick(QMouseEvent *event);

public:
    void addLine(Point* p1, Point* p2);
    void addShape();
    Point* addPoint(float x, float y, float z);

private:
    Model *model;
    DrawingOpenGLWidget *view;
    ModeController *modeController;
};

#endif
