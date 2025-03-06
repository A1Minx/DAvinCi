#ifndef MODECONTROLLER_H
#define MODECONTROLLER_H



#include "M_ModeInterface.h"
#include "M_DrawLine.h"
#include "M_DrawPoint.h"
#include "M_Selection.h"


//forward declarations
class Controller;
class DrawingOpenGLWidget;



class ModeController
{
public:
    ModeController(Controller *controller);

    enum class Modes {
        Selection,
        DrawLine,
        DrawPoint
    };

    void changeMode(Modes mode);

    M_ModeInterface* getCurrentMode();

private:
    Controller *controller;

    M_ModeInterface *CurrentMode;

    //available Modes. All get initialized once during startup when the ModeController gets constructed to avoid reoccuring heap allocations
    M_DrawLine *drawLine;
    M_DrawPoint *drawPoint;
    M_Selection *selection;


};

#endif
