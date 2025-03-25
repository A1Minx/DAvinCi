#ifndef MODECONTROLLER_H
#define MODECONTROLLER_H





class M_ModeInterface;
class M_DrawLine;
class M_DrawPoint;
class M_Selection;


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

    void reConfigureView();

private:
    Controller *controller;

    M_ModeInterface *CurrentMode;
    Modes currentModeType;

    //available Modes. All get initialized once during startup when the ModeController gets constructed to avoid reoccuring heap allocations
    M_DrawLine *drawLine;
    M_DrawPoint *drawPoint;
    M_Selection *selection;


};

#endif
