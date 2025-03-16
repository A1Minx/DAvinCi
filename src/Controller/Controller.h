#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <Model.h>
#include <QMouseEvent>
#include <ModeController.h>
#include <memory>

class View_OpenGLWidget;
class Model;
class ModeController;

class Controller : public QObject
{
    Q_OBJECT

public:
    Controller(Model *model, View_OpenGLWidget *view);
    void setView(View_OpenGLWidget *view);

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
    void addLine(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2);
    void addShape();
    std::shared_ptr<Point> addPoint(float x, float y, float z);

    void addTempLine(float x1, float y1, float z1, float x2, float y2, float z2);
    void removeTempLines();

    void addTempPoint(float x, float y, float z);
    void removeTempPoints();

    std::shared_ptr<Point> getNearestPoint(float x, float y, float z);
    
    void setMouseTracking(bool tracking);

private:
    Model *model;
    View_OpenGLWidget *view;
    ModeController *modeController;
};

#endif
