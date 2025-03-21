#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <Model.h>
#include <QMouseEvent>
#include <ModeController.h>
#include <memory>
#include <Point.h>
#include <Spec.h>

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
    void addLine(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2, std::shared_ptr<LineSpec> spec);
    void addShape();
    std::shared_ptr<Point> addPoint(float x, float y, float z, std::shared_ptr<PointSpec> spec);

    void addTempLine(float x1, float y1, float z1, float x2, float y2, float z2, float color[4], float width);
    void removeTempLines();

    void addTempPoint(float x, float y, float z, float color[4], float size);
    void removeTempPoints();

    std::shared_ptr<Point> getNearestPoint(float x, float y, float z);
    
    void setMouseTracking(bool tracking);

    std::shared_ptr<PointSpec> getCurrPointSpec();
    std::shared_ptr<LineSpec> getCurrLineSpec();

    void setCurrPointSpec(std::shared_ptr<PointSpec> spec);
    void setCurrLineSpec(std::shared_ptr<LineSpec> spec);

private:
    Model *model;
    View_OpenGLWidget *view;
    ModeController *modeController;

    std::shared_ptr<PointSpec> currPointSpec;
    std::shared_ptr<LineSpec> currLineSpec;
};

#endif
