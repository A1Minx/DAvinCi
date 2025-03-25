#ifndef M_MODEINTERFACE_H
#define M_MODEINTERFACE_H

#include <QMouseEvent>
#include <QVector3D>
#include <memory>
#include "Controller.h"
#include "Point.h"
#include <QDebug>

class M_ModeInterface {
public:
    M_ModeInterface(Controller *controller) : controller(controller) {};
    virtual ~M_ModeInterface() = default;
    
    // Mouse Events
    virtual void onMouseClick(QMouseEvent *event, const QVector3D& worldPos) = 0;
    virtual void onMouseMove(QMouseEvent *event, const QVector3D& worldPos) = 0;


    // Default Implementations 
    virtual inline std::shared_ptr<Point> nearestPointSelection(QMouseEvent *event, const QVector3D& worldPos, char hiddenAxis) {

        // TODO: here is another switch in the controller, check if one of those is redundant
        switch (hiddenAxis) {
            case 'x':
                nearestPoint = controller->getNearestPoint(worldPos.y(), worldPos.z(), 'x');
                break;
            case 'y':
                nearestPoint = controller->getNearestPoint(worldPos.x(), worldPos.z(), 'y');
                break;
            case 'z':
                nearestPoint = controller->getNearestPoint(worldPos.x(), worldPos.y(), 'z');
                break;
            case '0':
                nearestPoint = controller->getNearestPoint(worldPos.x(), worldPos.y(), worldPos.z());
                break;
            default:
                qDebug() << "Invalid hidden axis";
                break;
        }
        return nearestPoint;
    }
        

protected:
    Controller *controller;

    std::shared_ptr<Point> nearestPoint;
    bool firstClick;

    

    //TODO: make configurable
    float tempColor[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    float tempSize = 10.0f;
};

#endif
