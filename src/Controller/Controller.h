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
    void readSQLLines();
    void readSQLPoints();
    void handleMouseClick(QMouseEvent *event);
    void handleMouseMove(QMouseEvent *event);

public:
    void addLine(Point* p1, Point* p2);
    void addShape();
    Point* addPoint(float x, float y, float z);

    void addTempLine(float x1, float y1, float z1, float x2, float y2, float z2);
    void removeTempLines();

    void addTempPoint(float x, float y, float z);
    void removeTempPoints();

    std::shared_ptr<Point> getNearestPoint(float x, float y, float z);
    
    void setMouseTracking(bool tracking);

private:
    Model *model;
    DrawingOpenGLWidget *view;
    ModeController *modeController;
};

#endif
