TEMPLATE = app
TARGET = DAvinCi

QT = core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets gui

LIBS *= -lpq

SOURCES += \
    main.cpp \
    ./View/mainWindow.cpp \
    ./View/View_OpenGLWidget.cpp \
    ./View/Orth_XY_OpenGLWidget.cpp \
    ./View/Orth_XZ_OpenGLWidget.cpp \
    ./View/Orth_YZ_OpenGLWidget.cpp \
    ./Controller/Controller.cpp \
    ./Controller/ModeController.cpp \
    ./Controller/M_DrawLine.cpp \
    ./Controller/M_DrawPoint.cpp \
    ./Controller/M_Selection.cpp \
    ./Model/Model.cpp \
    ./Model/SQLServer.cpp \
    ./Model/Point.cpp \
    ./Model/Object3D.cpp \
    ./Model/Line.cpp \
    ./Model/Spec.cpp

HEADERS += \
    ./View/mainWindow.h \
    ./View/View_OpenGLWidget.h \
    ./View/Orth_XY_OpenGLWidget.h \
    ./View/Orth_XZ_OpenGLWidget.h \
    ./View/Orth_YZ_OpenGLWidget.h \
    ./Controller/Controller.h \
    ./Controller/M_DrawLine.h \
    ./Controller/ModeController.h \
    ./Controller/M_ModeInterface.h \
    ./Controller/M_DrawPoint.h \
    ./Controller/M_Selection.h \
    ./Model/Model.h \
    ./Model/SQLServer.h \
    ./Model/BasicObject.h \
    ./Model/Point.h \
    ./Model/Object3D.h \
    ./Model/Line.h \
    ./Model/Spec.h

INCLUDEPATH +=  ./Controller ./View ./Model /usr/include /usr/lib/x86_64-linux-gnu

CONFIG += c++17
