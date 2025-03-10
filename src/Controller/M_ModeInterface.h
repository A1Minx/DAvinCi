#ifndef M_MODEINTERFACE_H
#define M_MODEINTERFACE_H

#include <QMouseEvent>
#include <QVector3D>

class M_ModeInterface {
public:
    virtual ~M_ModeInterface() = default;
    

    virtual void onMouseClick(QMouseEvent *event, const QVector3D& worldPos) = 0;
    virtual void onMouseMove(QMouseEvent *event, const QVector3D& worldPos) = 0;
};

#endif
