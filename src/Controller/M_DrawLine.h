#ifndef M_DRAWLINE_H
#define M_DRAWLINE_H

#include "M_ModeInterface.h"

class Controller;
class DrawingOpenGLWidget;


class M_DrawLine : public M_ModeInterface {
public:
    M_DrawLine(Controller *controller, DrawingOpenGLWidget *view);

    void onMouseClick(QMouseEvent *event) override;

private:
    Controller *controller;
    DrawingOpenGLWidget *view;
    bool firstClick = true;
    float x1, y1;
};

#endif
