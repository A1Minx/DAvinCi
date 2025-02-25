#ifndef M_MODEINTERFACE_H
#define M_MODEINTERFACE_H

#include <QMouseEvent>

class M_ModeInterface {
public:
    virtual ~M_ModeInterface() = default;
    virtual void onMouseClick(QMouseEvent *event) = 0;
};

#endif
