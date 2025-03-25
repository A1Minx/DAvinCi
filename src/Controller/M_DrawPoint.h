#ifndef M_DRAWPOINT_H
#define M_DRAWPOINT_H

#include "M_ModeInterface.h"
#include "Point.h"

class Controller;

class M_DrawPoint : public M_ModeInterface {
public:
    M_DrawPoint(Controller *controller);

    void onMouseClick(QMouseEvent *event, const QVector3D& worldPos) override;
    void onMouseMove(QMouseEvent *event, const QVector3D& worldPos) override;

private:

};

#endif

