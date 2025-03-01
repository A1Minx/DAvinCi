#ifndef M_SELECTION_H
#define M_SELECTION_H

#include "M_ModeInterface.h"

class Controller;



class M_Selection : public M_ModeInterface {
public:
    M_Selection(Controller *controller);

    void onMouseClick(QMouseEvent *event) override;

private:
    Controller *controller;

};

#endif
