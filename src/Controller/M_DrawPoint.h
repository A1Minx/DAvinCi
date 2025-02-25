#ifndef M_DRAWPOINT_H
#define M_DRAWPOINT_H

#include "M_ModeInterface.h"

class Controller;
class DrawingOpenGLWidget;


class M_DrawPoint : public M_ModeInterface {
public:
    M_DrawPoint(Controller *controller, DrawingOpenGLWidget *view);

    void onMouseClick(QMouseEvent *event) override;

private:
    Controller *controller;
    DrawingOpenGLWidget *view;
};

#endif

