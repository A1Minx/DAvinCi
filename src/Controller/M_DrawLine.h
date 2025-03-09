#ifndef M_DRAWLINE_H
#define M_DRAWLINE_H

#include "M_ModeInterface.h"
#include "Point.h"
class Controller;


class M_DrawLine : public M_ModeInterface {
public:
    M_DrawLine(Controller *controller);

    void onMouseClick(QMouseEvent *event, const QVector3D& worldPos) override;

private:
    Controller *controller;
    bool firstClick = true;
    float x1, y1;
    Point *p1, *p2;
};

#endif
