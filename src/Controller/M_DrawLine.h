#ifndef M_DRAWLINE_H
#define M_DRAWLINE_H
#include <memory>

class Controller;
class Point;

#include "M_ModeInterface.h"
#include <QMouseEvent>
#include <QVector3D>

class M_DrawLine : public M_ModeInterface {
public:
    M_DrawLine(Controller *controller);

    void onMouseClick(QMouseEvent *event, const QVector3D& worldPos) override;
    void onMouseMove(QMouseEvent *event, const QVector3D& worldPos) override;
private:
    std::shared_ptr<Point> p1, p2;
    
};

#endif
